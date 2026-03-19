#!/bin/bash
# check_purity.sh — Automated architecture purity audit
#
# Verifies that pure zones (src/math/, src/systems/) follow P1-P5 rules.
# See .context/standards/PURITY_RULES.md for rule definitions.
#
# Usage: ./scripts/check_purity.sh
# Exits 0 on clean, 1 on violations.

set -e

cd "$(dirname "$0")/.."

ERRORS=0

echo "=== Purity Audit ==="

# --------------------------------------------------------------------------
# P1: No GL in pure code
# Forbidden: GLES3, glCreate, glBind, glGen, glDelete, glUniform, glDraw, GL_
# Exclude comments and string literals
# --------------------------------------------------------------------------
echo ""
echo "--- P1: No GL calls in pure zones ---"
P1_HITS=$(grep -rn \
    -e 'GLES3' -e 'glCreate' -e 'glBind' -e 'glGen' \
    -e 'glDelete' -e 'glUniform' -e 'glDraw' \
    --include="*.c" --include="*.h" \
    src/math/ src/systems/ 2>/dev/null \
    | grep -v '^\s*//' \
    | grep -v '^\s*\*' \
    | grep -v '// P1-exempt' \
    || true)

# GL_ is too broad for grep (matches GL_TRIANGLE etc but also unrelated).
# Only check for GL_ followed by uppercase (actual GL enums)
P1_GL=$(grep -rn -P '\bGL_[A-Z]' \
    --include="*.c" --include="*.h" \
    src/math/ src/systems/ 2>/dev/null \
    | grep -v '^\s*//' \
    | grep -v '^\s*\*' \
    || true)

P1_ALL="${P1_HITS}${P1_GL}"
P1_ALL=$(echo "$P1_ALL" | sed '/^$/d')

if [ -n "$P1_ALL" ]; then
    echo "  VIOLATION: GL calls found in pure zones:"
    echo "$P1_ALL" | head -20
    ERRORS=$((ERRORS + 1))
else
    echo "  PASS"
fi

# --------------------------------------------------------------------------
# P2: No mutable static VARIABLES in pure code
# Forbidden: static variables (not functions) that aren't const/inline
# Strategy: match "static TYPE name" lines, exclude function definitions
#           (which have '(' after the name)
# --------------------------------------------------------------------------
echo ""
echo "--- P2: No mutable statics in pure zones ---"
P2_HITS=$(grep -rn '^static ' \
    --include="*.c" \
    src/math/ src/systems/ 2>/dev/null \
    | grep -v 'static const ' \
    | grep -v 'static inline ' \
    | grep -v '// P2-exempt' \
    | grep -v '(' \
    || true)

# The above filters out function definitions (any line containing '(' is a
# function def/decl). What remains are static variable declarations.

if [ -n "$P2_HITS" ]; then
    echo "  VIOLATION: Mutable statics found in pure zones:"
    echo "$P2_HITS" | head -20
    ERRORS=$((ERRORS + 1))
else
    echo "  PASS"
fi

# --------------------------------------------------------------------------
# P3: No heap allocation in pure code
# Forbidden: malloc(, calloc(, realloc(, free( — actual function calls
# Exclude comments (lines starting with *, //, or inside /* */)
# --------------------------------------------------------------------------
echo ""
echo "--- P3: No heap allocation in pure zones ---"
P3_HITS=$(grep -rn -P '\b(malloc|calloc|realloc|free)\s*\(' \
    --include="*.c" --include="*.h" \
    src/math/ src/systems/ 2>/dev/null \
    | grep -v '// P3-exempt' \
    || true)

# Filter out comments: lines where the match is inside a comment
P3_REAL=""
while IFS= read -r line; do
    [ -z "$line" ] && continue
    content=$(echo "$line" | sed 's|^[^:]*:[0-9]*:||')
    # Skip comment lines
    if echo "$content" | grep -qP '^\s*//' ; then continue; fi
    if echo "$content" | grep -qP '^\s*\*' ; then continue; fi
    if echo "$content" | grep -qP '^\s*/\*' ; then continue; fi
    P3_REAL="${P3_REAL}${line}
"
done <<< "$P3_HITS"
P3_REAL=$(echo "$P3_REAL" | sed '/^$/d')

if [ -n "$P3_REAL" ]; then
    echo "  VIOLATION: Heap allocation found in pure zones:"
    echo "$P3_REAL" | head -20
    ERRORS=$((ERRORS + 1))
else
    echo "  PASS"
fi

# --------------------------------------------------------------------------
# P4: No I/O in pure code (except #ifdef DEBUG)
# Forbidden: printf(, fprintf(, puts( — actual function calls
# Exclude string literals, comments
# --------------------------------------------------------------------------
echo ""
echo "--- P4: No I/O in pure zones ---"
P4_HITS=$(grep -rn -P '\b(printf|fprintf|puts)\s*\(' \
    --include="*.c" \
    src/math/ src/systems/ 2>/dev/null \
    | grep -v '// P4-exempt' \
    | grep -v '^\s*//' \
    | grep -v '^\s*\*' \
    || true)

if [ -n "$P4_HITS" ]; then
    echo "  VIOLATION: I/O calls found in pure zones:"
    echo "$P4_HITS" | head -20
    ERRORS=$((ERRORS + 1))
else
    echo "  PASS"
fi

# --------------------------------------------------------------------------
# P5: No app_state reference in pure code
# Forbidden: app_state, g_state
# --------------------------------------------------------------------------
echo ""
echo "--- P5: No app_state in pure zones ---"
P5_HITS=$(grep -rn \
    -e '\bapp_state\b' -e '\bg_state\b' \
    --include="*.c" --include="*.h" \
    src/math/ src/systems/ 2>/dev/null \
    | grep -v '// P5-exempt' \
    | grep -v '^\s*//' \
    | grep -v '^\s*\*' \
    || true)

if [ -n "$P5_HITS" ]; then
    echo "  VIOLATION: app_state/g_state found in pure zones:"
    echo "$P5_HITS" | head -20
    ERRORS=$((ERRORS + 1))
else
    echo "  PASS"
fi

# --------------------------------------------------------------------------
# Summary
# --------------------------------------------------------------------------
echo ""
echo "==========================================="
if [ $ERRORS -gt 0 ]; then
    echo "  RESULT: FAIL — $ERRORS purity violation(s)"
    exit 1
else
    echo "  RESULT: PASS — all pure zones clean (P1-P5)"
    exit 0
fi

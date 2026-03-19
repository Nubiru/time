# Translation Voice Guide — Time Project

## The Voice of Time

Time is not a utility app. Its text speaks with the voice of humanity's greatest minds — Zarathustra, Rumi, Gandhi, Lao Tzu, Meeus, Arguelles. Every translated string must carry this spirit.

## Principles

### 1. Poetry Over Precision
- "Loading..." → Spanish: "Despertando..." (Awakening), not "Cargando..."
- "Jump complete" → "Has llegado" (You have arrived), not "Salto completo"
- "Error" can stay "Error" — dignity in simplicity

### 2. Warmth Over Formality
- "View changed" → "Nueva perspectiva" (New perspective), not "Vista cambiada"
- "Layer toggled" → "Capa revelada" (Layer revealed), not "Capa alternada"
- Address the user as a fellow traveler, not a customer

### 3. Cultural Resonance
- "Fun Fact" → "Dato Revelador" (Revealing fact), not "Dato curioso"
- "Achievement" → "Logro de la Humanidad" (Achievement of Humanity)
- Each culture's calendar names stay in their original language when sacred

### 4. Cosmic Framing
- "Space View" → "Vista del Cosmos" — cosmos, not just space
- "Welcome to Time" → "Bienvenido al Tiempo" — Time as a living entity
- Everything connects to the larger pattern

### 5. Dignity in All Languages
- Accessibility text must be clear AND beautiful, never clinical
- Error messages must be gentle, never blaming
- Sacred text requires extra care — consult native speakers

## What NOT to Do

- Machine translate and ship. Every string needs a human eye.
- Use English grammar structures forced into another language.
- Strip metaphor from poetic strings.
- Use informal/slang register — Time speaks with quiet authority.
- Truncate or simplify sacred terminology.

## Process

1. Start from `data/translations/en.json` (master)
2. Create `data/translations/{locale}.json`
3. Run `python3 scripts/gen_translations.py` to generate C header
4. Gabriel or native speaker verifies quality
5. Merge generated header into i18n.c when approved

## Priority Languages

| Tier | Languages | Verifier |
|------|-----------|----------|
| P0 | EN, ES, AR, ZH | Gabriel (ES), friend (AR, ZH) |
| P1 | HI, JA, FR, PT, HE | Gabriel (HE, PT) |
| P2 | DE, RU, KO, TH, TR, ID | community |
| P3 | MY, AM, BN, VI, SW, FA | community |

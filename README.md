# Demon Coach

A Geode mod for Geometry Dash 2.2081 that provides personalized Demon recommendations.

## Target

- Geometry Dash: 2.2081
- Geode: 5.11.2 (the current stable Geode release is primarily for 2.2081)
- Android64 / Android32 builds can be produced by the Geode build workflow.

## Features planned in this base

- First-run player profile
- Hardest-level field
- Favorite gamemode
- Learny / skill-based / balanced preference
- Skill tags such as nerve control, chokepoints, timings, wave, ship, etc.
- Configurable daily/weekly/monthly frequencies
- Minimum-like filter defaulted to 4000
- Pointercrate list-demon API connectivity
- AREDL API connectivity
- Persistent profile storage

## Important data-source note

Pointercrate exposes a documented API. AREDL exposes an API v2. GDDL/GDDL data availability and fields can change independently of Geode. The recommendation engine should only consume public data and should gracefully fall back when a source is unavailable.

This repository intentionally keeps the networking layer separated from the UI so list/API changes can be handled without rewriting the menu.

## Build

The included GitHub Actions workflow uses the official Geode build action. Push the repository to GitHub and open the Actions tab. The resulting `.geode` artifact is produced by the workflow.

No source-file renaming is required.

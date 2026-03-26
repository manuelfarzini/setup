- install opencode + neovim plugin (done)
- setup provider + model (/connect, /models)
- choose cheap/good default model (e.g. gpt-5.4-mini or similar)

- create AGENTS.md in repo root
  - define module scope (paths)
  - forbid full repo scanning
  - define coding conventions + boundaries

- create opencode.json
  - permissions:
    - "edit": "ask"
    - "bash": "ask"
  - prevent runaway tool usage

- workflow:
  - use Plan first (design, reasoning)
  - then Build (apply changes)
  - never start directly in Build for complex tasks

- always constrain scope in prompt
  - list target files explicitly
  - say "do not explore outside this module"

- use neovim context injection
  - @this (selection)
  - @buffer
  - avoid full repo context

- avoid expensive prompts
  - no "analyze entire repo"
  - no blind exploration

- optionally:
  - .opencode/commands/*.md for reusable tasks
  - custom skills if needed
  - reuse sessions (continue previous work)

- mental model:
  - cost = tokens + tool calls
  - less context + less wandering = cheaper

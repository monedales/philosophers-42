# Agent Skills for 42 Projects

This directory contains custom skills for GitHub Copilot to help with 42 School projects.

## Available Skills

### 1. 🔍 Norminette Checker
**Location**: `norminette-checker/`

Automatically checks and fixes code according to 42 School's Norminette standards.

**Triggers**:
- Mentions of "norm", "norminette", "42 style"
- Creating or modifying .c or .h files
- Code review requests

**Capabilities**:
- Validates code against Norminette rules
- Suggests fixes for common violations
- Ensures proper formatting and structure

### 2. 🏗️ 42 Patterns
**Location**: `42-patterns/`

Applies common patterns, structures, and best practices for 42 projects.

**Triggers**:
- Starting new projects
- Mentions of "42 patterns", "project structure"
- Setting up Makefiles or project architecture

**Capabilities**:
- Standard project structure templates
- Makefile patterns
- Error handling patterns
- Memory management best practices
- Threading patterns
- .gitignore templates

### 3. 🍝 Philosophers Guide
**Location**: `philosophers-guide/`

Specific guidance for the Dining Philosophers (philo) project.

**Triggers**:
- Mentions of "philosophers", "philo", "dining"
- Threading or synchronization questions
- "deadlock", "race condition" mentions

**Capabilities**:
- Project-specific structure and patterns
- Deadlock and data race prevention strategies
- Monitor thread implementation
- Testing strategies
- Common issues and solutions
- Precise timing implementations

## How Skills Work

When you interact with GitHub Copilot, it automatically:
1. Detects relevant skills based on your prompt or context
2. Loads the skill's instructions and resources
3. Follows the guidelines to provide accurate, project-specific assistance

## Using Skills

Simply mention relevant keywords in your prompts:

```
"Check this code against the norminette"
"Help me set up a new 42 project structure"
"Why are my philosophers deadlocking?"
"Show me the 42 Makefile pattern"
```

Copilot will automatically use the appropriate skill(s) to assist you.

## Extending Skills

To create new skills:
1. Create a new folder in `.claude/skills/`
2. Add a `skill.md` file with:
   - Purpose description
   - When to use triggers
   - Key concepts and patterns
   - Examples and best practices
3. Copilot will automatically pick it up

## Skills Compatibility

These skills are compatible with:
- GitHub Copilot coding agent
- Copilot CLI
- VS Code agent mode (Insiders and stable)

## Notes

- Skills are loaded contextually - you don't need to explicitly call them
- Multiple skills can be active simultaneously
- Skills work best with clear, specific prompts
- The more you use skills, the better Copilot understands your project context

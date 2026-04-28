# Git workflow for keeping a linear history

### Base

origin   = my GitHub fork
upstream = the official repository

### First check

Always start with:

```
    git status --short --branch
    git fetch origin
    git fetch upstream
    git status --short --branch
```

### Case A − Everything is clean and aligned

State:

```
    clean working tree
    0 ahead
    0 behind
```

I just work normally:

```
    git add ...
    git commit -m "..."
    git push origin master-dev
```

### Case B − Clean working tree, no local commits, upstream is ahead

Fast-forward from upstream

```
    git merge --ff-only upstream/master-dev
    git push origin master-dev
```

### Case C − Clean working tree, no local commits, origin is ahead

- Typical case: I worked from another machine.

Fast-forward from origin

```
    git merge --ff-only origin/master-dev
```

### Case D − Clean working tree, both origin and upstream are ahead

First compare origin and upstream:

```
    git rev-list --left-right --count origin/master-dev...upstream/master-dev
```

- Output: `0 N`, upstream is ahead of origin.

```
    git merge --ff-only upstream/master-dev
    git push origin master-dev
```

- Output: `N 0`, origin is ahead of upstream.

```
    git merge --ff-only origin/master-dev
```

- Output: `N M`, origin and upstream have diverged
  - do not merge automatically,
  - reason about it / rebase carefully.

### Case E − Dirty working tree, origin is ahead

Typical case: I have uncommitted changes on this machine, but I already worked and pushed from another one.

Save local work, update, then reapply it:

```
    git stash push -u
    git merge --ff-only origin/master-dev
```

Optionally check upstream:

```
    git rev-list --left-right --count HEAD...upstream/master-dev
```

Output: `0 N`, upstream is ahead

```
    git merge --ff-only upstream/master-dev
    git push origin master-dev
```

then recover local work:

```
    git stash pop
```

If there are no conflicts:

```
    git add ...
    git commit -m "..."
    git push origin master-dev
```

If there are conflicts:

```
    # fix files manually
    git add ...
    git commit -m "..."
    git push origin master-dev
```

### Case F − Local commits not pushed, origin is ahead

Typical case: I committed on this machine, but I also worked and pushed from another one.

Situation:

```
    local master-dev:   A -- B -- my_commit
    origin/master-dev:  A -- B -- C -- D
```

I want:

```
    A -- B -- C -- D -- my_commit
```

Rebase onto origin/master-dev:

```
    git rebase origin/master-dev
```

If there are conflicts:

```
    # fix files manually
    git add ...
    git rebase --continue
```

then:

```
    git push origin master-dev
```

### Case G − Local commits not pushed, upstream is ahead

Rebase onto upstream/master-dev:

```
    git rebase upstream/master-dev
```

If there are conflicts:

```
    # fix files manually
    git add ...
    git rebase --continue
```

then update my fork:

```
    git push --force-with-lease origin master-dev
```

## General rules

1. Remote is ahead and I have no local commits:

```
    fast-forward
```

2. Local commits and remote is ahead:

```
    rebase
```

3. Dirty working tree:

```
    first stash / commit / discard, then update
```

4. Origin is ahead because I worked from another machine:

- If I have no local commits:

```  
    git merge --ff-only origin/master-dev
```

- If I have local commits:

```
    git rebase origin/master-dev
```

5. Upstream is ahead:

- If I have no local commits:

```
    git merge --ff-only upstream/master-dev
```

- If I have local commits:

```
    git rebase upstream/master-dev
```

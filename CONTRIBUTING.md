# Code of Conduct

The following code of conduct must be followed in all places associated with this repository. This includes issues, pull requests, and all files contained within the repository.

In addition to following GitHub's terms of service and any laws applicable to you, please:


# Standards




# Pull Requests

When submitting pull requests, 
please default to submitting to the `develop` branch and never submit directly to the `master` branch. 
This allows us to correctly manage versioning. 
If you submit a pull request directly to the `master` branch, you will be asked to change the target to `develop` (or another applicable branch).

All pull requests should be tested by Travis-CI. 
Please note whether you updated the CI or whether no change was needed. 
If for some reason a new, untested feature is implemented,
but you are unable to implement the necessary CI, 
explain why and how it can be manually tested.

## Template
The following template is not required, but if you do not use it, please be sure to include all answers to all of the questions in some other way.

**Does your pull request resolve or partially resolve an issue?** 
\# / No.

**Does your pull request implement any breaking changes?**
Yes / No.

**If breaking changes are implemented, please describe:**

**Testing:**  
This pull request:
[ ] Alters the existing CI in some way.
[ ] Adds a new step to the CI.
[ ] Does not introduce any features that the CI could not already test.
[ ] Is not accompanied by necessary CI changes due to some limitation described below. (Please also describe how new features can be manually tested.)

**Other information:**
Anything else you want to say.

# GitHub Issues

Issues fall into three categories:
* Bug report
* Feature request
* Documentation issue

When submitting issues, please be specific. We can only resolve a bug if we know what about the program isn't working, implement a feature if we know what aspect is being improved, and clarify documentation if we know what part is unclear.

Below are outlines for determining what your issue qualifies as and how to report them. When submitting an issue, please specify which of these three categories you think it belongs in. We understand that the three categories can overlap, so don't worry too much if you aren't sure if the category you chose is appropriate. Each section also provides a template; these are just to help people know what to write, and their use is not strictly required (although it may help us address the issue faster).

## Bug report

When submitting a bug report, please make sure to include any information 
necessary for us to reproduce the bug. If we can't reproduce it, it will be
much harder to diagnose and solve the issue.

An issue is a bug report if:
* The code does not run or only partially runs.
* The code does not build.
* A command associated with the code fails to run despite matching the documentation.
* The code takes an inordinately long amount of time to run, taking hardware into account.
* The code gives no output either to a binary file, a log file, or the terminal, when it should be giving some kind of output there.
* A command that should give consistent output gives different output each time.
* The result of a command directly contradicts what the documentation says should occur.

An issue is not a bug report if:
* The code does not interface with an environment that the documentation does not specify it will interface with. (Feature request)
* The code is missing the ability to do something you think it should be able to do, but the documentation does not specify it is capable of. (Feature request)
* The documentation is unclear but the code does not give results that directly contradict it. (Documentation issue)

### Template

I am submitting a **bug report**.

**This bug occurs in:**
make / realizeCascades / CI / specific file / etc.

**Expected behavior:**
____ should ____.

**Current behavior:**
____ instead does ____.

**Steps to reproduce:**
1. Do thing
2. Do thing
3. Result

**Other Information:**
Anything else you want to say.

**Relevant Output:**
Provide a log file, text from terminal, "No output", etc.

## Feature request

An issue is a feature request if:
* You are requesting for the code to interface in a new way that it currently does not, such as a new command or argument.
* You are proposing a particular way to increase the speed of the code.
* You are pointing out where the code could be more user-friendly.
* You are otherwise requesting for the code to do something it is not yet written to do.

An issue is not a feature request if:
* It does not affect the code, only the documentation. (Documentation issue)
* It is to fix unexpected behavior. (Bug report)
* You are providing the feature you are requesting. (Pull request)

### Template

I am submitting a **feature request**.

**The feature I am requesting is for:**
make / realizeCascades / CI / specific file / Something new / etc.

**I am requesting:**
A completely new feature / An improvement on an existing feature / etc.

**Ideas for implementation:**
(Optional)

**Other Information:**
Anything else you want to say.

## Documentation

An issue is a documentation issue if:
* A command mentioned in the documentation cannot be found.
* You would like an example and there is no similar example in the documentation.
* There is a part of the documentation you are asking for us to clarify.
* There are spelling and grammar errors, missing images, or broken links which you do not know how best to fix with a pull request.

An issue is not a documentation issue if:
* You provide fixed wording, spelling, and/or grammar for all issues you point out. (Pull request)
* The code attempts to run but fails. (Bug report)
* You are looking for a way to do something that you do not know exists and is not mentioned in the documentation. (Feature request)

### Template

I am submitting a **documentation issue**.

**The file(s) in question is/are:**
README.md / CONTRIBUTING.md / LICENSE / etc.

**The problem is in the following category/categories:**
Clarity / Examples / Broken links and images / Typos, spelling, and grammar / Undocumented Information / Out-of-date / Other

**Description of the problem:**
Describe whatever is wrong with the documentation or could otherwise be improved.

# GitHub Discussions

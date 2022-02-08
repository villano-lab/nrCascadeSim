## Future Releases (v1.3.3) Date XX.XX.XX 

* PR #91 migrated to CMake (Issues #85, #64, #26)
* PR #95 added proper coverage behavior to CMake (Issues #94, #93, #92)

## Releases (v1.3.2) Date 22.01.08 

* PR #77 extended testing to more input files (Issue #35)
* PR #78 updated documentation to systematicall include units of all parameters (Issue #74)
* PR #79 updated documentation to clarify a requirement when installing ROOT dependency on WSL (Issue #76)
* PR #80 improved the testing on the included example notebook (Issue #65)

## Release (v1.3.1) Date 21.12.30 

* PR #62 added fail-on-warning flag for readthedocs (Issue #41)
* PR #68 used breathe to bridge doxygen documentation to readthedocs (Issue #47)
* PR #69 added code coverage and more testing (Issue #49, #44)

## Release (v1.3.0) Date 21.12.27

* PR #54 Move MersenneTwister to built-in C++ function. (Issue #27)
	* The software now requires C++ 11 or newer.
* PR #55 fixed sphinx documentation warnings. (Issue #46)
* PR #56 Travis now checks if files are empty. (Issue #50)
* PR #57 adds testing for c++1--c++17 under ROOT 6.24.08
* PR #58 Improvements to documentation:
	* CONTRIBUTING is no longer cluttered with contents of other documents such as templates and the code of conduct.
	* Syntax for verbosity flags was clarified. (See comments in issue #48)
	* An error in the lifetime units in the documentation was fixed. (All code was already consistent in this regard. Issue #53)
	* CONTRIBUTING now outlines a workflow for changes to be made. (Issue #34)
	* Added arXiv badge. (See issue #37)
	* Include python packages necessary for building docs in the provided yml file.
	* Ignore doc build output and remove build output from repository.
	* Other formatting improvements.
* PR #61 fixes regex to extend to OSX (issue #29)

## Release (v1.2.4) Date 21.12.15

* PR #51 Hotfix to remove the bug introduced by verbosity (solves issue #12)

## Release (v1.2.3) Date 21.11.19

* Hotfix to add submitted tag for JOSS

## Release (v1.2.2) Date 21.11.14

* Hotfix to fix various typos

## Release (v1.2.1) Date 21.11.14

* Hotfix to update release notes. 

## Release (v1.2.0) Date 21.11.13

* PR #38 adds documentation on readthedocs (solves issue #23)
* PR #40 fixes up example (solves issue #7)


## Release (v1.1.3) Date 21.11.06

* PR #16 to lower default verbosity of realizeCascades (solves issue #12; bug introduced see v1.2.4)
* PR #20 adds more clarity to README documentation at top level (solves issue #8)

## Release (v1.1.1) Date 21.10.23

* PR #34 Hotfix to fix Zenodo DOI

## Release (v1.1.0) Date: 21.10.23

* PR #18 and #25 implementing JOSS paper updates (solves issue #11)
* PR #21 correcting the ROOT citation (solves issue #4)
* PR #19 updating the documentation on the mechanism for contributing in CONTRIBUTING.md (solves issue #9)
* PR #28 fix the c++17 bug for the Mersenne Twister in clang on OSX 10.14.6 (issue #27) --> had to re-open this issue PR #28 did NOT solve it
* PR #30 Got a Zenodo DOI (issue #24) 
* PR #17 Got merged accidentally with PR #30 and it partially addressed issue #10 
* PR #31 Got merged (closes out issue #10)

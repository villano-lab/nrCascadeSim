#Release Checklist

Please be sure to do the following when making a release.

1. get a new Zenodo DOI (update badges?)--one in README.md one in readthedocs: index.rst and 06_Citations.rst
2. update release page of readthedocs documentation `docs/source/07_Release_History.rst`
3. update citation page of readthedocs `docs/source/06_Citations.rst`
4. be sure the `stable` build of readthedocs points to the new release
5. update the front-facing README in the Version history and citations sections
6. update release notes RELEASENOTES.md
7. update `docs/source/index.rst` if badges changed
8. make a release on github
9. restore fail-on-warning on .readthedocs.yaml
10. be sure codecov website is switched to default to master branch

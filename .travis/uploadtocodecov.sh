#!/bin/bash
#Haha! This needs a key you do not have! So only I can use it.

#Compile the info
lcov --directory ~/build/nrCascadeSim_build/ --capture --output-file nrCascadeSim_coverage.info
genhtml nrCascadeSim_coverage.info
#Download the main uploader script
curl -Os https://uploader.codecov.io/latest/linux/codecov
#Check the uploader script
curl https://keybase.io/codecovsecurity/pgp_keys.asc | gpg --no-default-keyring --keyring trustedkeys.gpg --import
curl -Os https://uploader.codecov.io/latest/linux/codecov.SHA256SUM
curl -Os https://uploader.codecov.io/latest/linux/codecov.SHA256SUM.sig
gpgv codecov.SHA256SUM.sig codecov.SHA256SUM
shasum -a 256 -c codecov.SHA256SUM
#Since we got here, uploader script checks out, so run it
chmod +x codecov
./codecov -f nrCascadeSim_coverage.info -t ${CODECOV_TOKEN}

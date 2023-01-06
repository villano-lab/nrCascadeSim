#!/bin/bash
#Setup
#Test realizeAndSave and if the result is nonempty - Ge
cp ../levelfiles/v1_natGe_WSlow.txt ./inputfile.txt
bin/realizeAndSave >/dev/null
python3 isempty.py
# Ne
cp ../levelfiles/20ne_ngam.txt ./inputfile.txt
bin/realizeAndSave >/dev/null
python3 isempty.py
# Ar
cp ../levelfiles/40ar_ngam.txt ./inputfile.txt
bin/realizeAndSave >/dev/null
python3 isempty.py
# Si
cp ../levelfiles/Si28_ngam_all_cascades_rfmt_sorted.txt ./inputfile.txt
bin/realizeAndSave >/dev/null
python3 isempty.py
echo "realizeAndSave ran successfully."

#Test printIsotopeInfo and if the result is consistent
bin/printIsotopeInfo > isotope_output.txt
if ! cmp isotope_output.txt sample_outputs/sample_isotope_output.txt
then
	echo "error: printIsotopeInfo output changed!"
	exit 1
fi
echo "printIsotopeInfo ran successfully."

#Test readLevelfile and if the result is consistent for the same levelfile
bin/readLevelfile > levelfile_output.txt
if ! cmp levelfile_output.txt sample_outputs/sample_levelfile_output_Si28ngam.txt
then
	echo "error: readLevelfile output changed! (Did you supply the same levelfile as used for the test output?)"
	exit 1
fi
echo "readLevelfile ran successfully."

#Test fetchYieldModel and if the result is consistent
bin/fetchYieldModel > yield_output.txt
if ! cmp yield_output.txt sample_outputs/sample_yield_output.txt
then
	echo "error: fetchYieldModel output changed!"
	exit 1
fi
echo "fetchYieldModel ran successfully."
echo "All sub-tests successful."

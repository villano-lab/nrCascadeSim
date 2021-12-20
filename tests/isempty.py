import uproot
file = uproot.open('output.root') #Change this to change tested file.
if len(file['cascades'].keys()) == 0:
	raise ValueError('No keys found under `cascades` - file appears to be empty!')

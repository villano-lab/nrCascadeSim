import uproot
file = uproot.open('output.root') #Change this to change tested file.
if len(file['cascade'].keys()) == 0:
	raise ValueError('No keys found under `cascade` - file appears to be empty!')

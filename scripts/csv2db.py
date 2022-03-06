#Imports
import pandas as pd
import sqlite3 as sq
import os

#Open files
csv = pd.read_csv('nndc.csv')
#print(os.path.isfile('isotopes.db')) #debug line
db = sq.connect('isotopes.db')
cur = db.cursor()

elements = [
	"n", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca",
	"Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr",
	"Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd",
	"Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg",
	"Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm",
	"Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og"
]

#Set up command
command = "INSERT INTO Isotopes (Name, Element, Z, N, A, Delta, Separation) \nVALUES\n"
for idx,row in csv.iterrows():
	z = row["z"]
	n = row["n"]
	astr = str(z+n)
	element = elements[z]
	name = astr+element
	separation = str(row["neutronSeparationEnergy(keV)"]).split()[0]
	if separation == "nan":
		separation = "NULL"
	delta = str(float(str(row["massExcess(keV)"]).split()[0])/1000)
	if delta == "nan":
		delta = "NULL"
	command += ("\t ('"+name+"', '"+element+"', "+str(z)+", "+str(row["n"])+", "+astr+", "+delta+", "+separation+"),\n")
command = command[:-2] + ";"#Change last comma to semicolon, stripping the " \n" along the way.

#Print a preview of the formatted command before running.
print("COMMAND PREVIEW: \n================")
print(command.splitlines()[0])
print(command.splitlines()[1])
print(command.splitlines()[2])
print("...")
print(command.splitlines()[-1])

#Debug
print(command, file=open("log.txt", "w"))

#Run command and commit results.
print("\nRunning command...")
cur.execute(command)
db.commit()
print("Committed!")

db.close()
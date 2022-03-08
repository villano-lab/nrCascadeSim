#Imports
import pandas as pd
import sqlite3 as sq
import os

#Open files
csv = pd.read_csv('nndc.csv',usecols=["z","n","massExcess(keV)","halflife","neutronSeparationEnergy(keV)","firstTwoPlusEnergy(keV)"])
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

cur.execute("ALTER TABLE Isotopes ADD COLUMN Lifetime NVARCHAR(20);") #create a temporary column to put lifetime info into

#Set up command
command = "INSERT INTO Isotopes (Name, Element, Z, N, A, Delta, Separation, Lifetime) \nVALUES\n"
for idx,row in csv.query("firstTwoPlusEnergy(keV) != nan").iterrows():
	z = row["z"]
	n = row["n"]
	astr = str(z+n)
	element = elements[z]
	separation = str(row["neutronSeparationEnergy(keV)"]).split()[0]
	if separation == "nan":
		separation = "NULL"
	delta = float(str(row["massExcess(keV)"]).split()[0])/1000
	if delta == "nan":
		delta = "NULL"
	command += ("\t ('"+astr+element+"', '"+element+"', "+str(z)+", "+str(row["n"])+", "+astr+", "+delta+", "+separation+", '"+row["halflife"]+"),\n") #The "normal" entry
	command += ("\t ('"+astr+"c"+element+"', '"+element+"', "+str(z)+", "+str(row["n"])+", "+astr+", "+delta+", "+str(float(separation) - float(str(row["firstTwoPlusEnergy(kev)"]).split()[0]))+", '"+row["halflife"]+"),\n")
for idx,row in csv.query("firstTwoPlusEnergy(keV) == nan").iterrows():
	z = row["z"]
	n = row["n"]
	astr = str(z+n)
	element = elements[z]
	separation = str(row["neutronSeparationEnergy(keV)"]).split()[0]
	if separation == "nan":
		separation = "NULL"
	delta = float(str(row["massExcess(keV)"]).split()[0])/1000
	if delta == "nan":
		delta = "NULL"
	command += ("\t ('"+astr+element+"', '"+element+"', "+str(z)+", "+str(row["n"])+", "+astr+", "+delta+", "+separation+", '"+row["halflife"]+"),\n") #The "normal" entry

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
print("\nRunning generating command...")
cur.execute(command)
print("Generating command complete. Checking for metastable entries...")



cur.execute("ALTER TABLE Isotopes DROP COLUMN Lifetime;")
db.commit()
print("Committed!")
db.close()
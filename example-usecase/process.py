#Imports
import sys
sys.path.append('python')
from hist import process

#Si
process('./data/Sifile.root','./data/Si_real_Lind_s0.txt',seed=0,model='Lindhard')
process('./data/Sifile.root','./data/Si_real_Sor_s0.txt',seed=0,model='Sorensen')
process('./data/Sifile.root','./data/Si_small_Lind_s0.txt',seed=0,model='Lindhard',resolution=0.2)
process('./data/Sifile.root','./data/Si_small_Sor_s0.txt',seed=0,model='Sorensen',resolution=0.2)

#Ne
process('./data/Nefile.root','./data/Ne_real_Lind_s0.txt',seed=0,model='Lindhard')
process('./data/Nefile.root','./data/Ne_real_Sor_s0.txt',seed=0,model='Sorensen')
process('./data/Nefile.root','./data/Ne_small_Lind_s0.txt',seed=0,model='Lindhard',resolution=0.2)
process('./data/Nefile.root','./data/Ne_small_Sor_s0.txt',seed=0,model='Sorensen',resolution=0.2)

#Ar
process('./data/Arfile.root','./data/Ar_real_Lind_s0.txt',seed=0,model='Lindhard')
process('./data/Arfile.root','./data/Ar_real_Sor_s0.txt',seed=0,model='Sorensen')
process('./data/Arfile.root','./data/Ar_small_Lind_s0.txt',seed=0,model='Lindhard',resolution=0.2)
process('./data/Arfile.root','./data/Ar_small_Sor_s0.txt',seed=0,model='Sorensen',resolution=0.2)

#Ge
process('./data/Gefile.root','./data/Ge_real_Lind_s0.txt',seed=0,model='Lindhard')
process('./data/Gefile.root','./data/Ge_real_Sor_s0.txt',seed=0,model='Sorensen')
process('./data/Gefile.root','./data/Ge_small_Lind_s0.txt',seed=0,model='Lindhard',resolution=0.2)
process('./data/Gefile.root','./data/Ge_small_Sor_s0.txt',seed=0,model='Sorensen',resolution=0.2)
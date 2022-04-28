(English version follows)

Per installare os161 su ubuntu 20 occorre:

sono consigliati almeno 5 GB di disco libero (se si fa una macchina virtuale, configurarla con almeno 16 GB totali)

una volta copiato il file os161-kit.tgz nella cartella home ($HOME) di uno user (attenzione non Documents o Downloads)
i file vanno estratti con:

tar -zxvf os161-kit.tgz

fare successivamente 

cd os161-kit

Se mancasse il file os161-downloads.tgz
scaricare in os161-kit/os161-downloads, dal sito http://os161.eecs.harvard.edu/download/ i seguenti kit:

binutils-2.24+os161-2.1.tar.gz  
bmake-20101215.tar.gz           
gcc-4.8.3+os161-2.1.tar.gz      
gdb-7.8+os161-2.1.tar.gz
mk-20100612.tar.gz
os161-base-2.0.3.tar.gz
sys161-2.0.8.tar.gz

Ma attenzione. Sono stati modificati due file:
- gdb-7.8+os161-2.1/sim/common/sim-arange.h (cancellata la riga 76: #include "sim-arange.c")
- sys161-2.0.8/include/onel.h (in riga 49: aggiunto extern nella dichiarazione di extra_selecttime)

I file in os161-downloads sono già modificati. Se si volessero i file originali dal sito di Harvard, occorrerebbe rifare le modifiche.

Eseguire, nell'ordine, da questa cartella (os161-kit):
attenzione, il primo file (installazioni con apt-get install) va eseguito da uno user abilitato a fare "sudo"
occorre scrivere la password e, se richiesto, rispondere alcune volte yes ("y")

source os161-env-startup.sh
source os161-kit-setup.sh
source os161-kit-build.sh
source os161-kernel-build.sh

attenzione: dopo kit-build e' necessario modificare il PATH in .profile.
Ho evitato di tentare una modifica automatica con sed o altro strumento simile.
Consiglio inoltre di fare logout/login (o reboot) prima di usare os161 (eventualmente prima di eseguire os161-kernel-build.sh).

Per installare VSCode e configurarlo, fare riferimento alle istruzioni nell'apposito documento.


G.Cabodi

PS: ringrazio Francesco Cheinasso e Giuseppe D'Andrea per la collaborazione nella messa a punto degli script e nell'individuazione delle modifiche a gdb e sys161.

=============================================================================

In order to install os161 on ubuntu 20 you need:

at least 5 GB of free disk is recommended (if you make a virtual machine, configure it with at least 16 GB total)

once the os161-kit.tgz file has been copied to the home folder ($HOME) of a user (avoid Documents or Downloads) the files must be extracted with:

tar -zxvf os161-kit.tgz

then do to the os161-kit folder:

cd os161-kit

If the os161-downloads.tgz file is missing, download in os161-kit/os161-downloads, from the site http://os161.eecs.harvard.edu/download/ the following kits:

binutils-2.24 + os161-2.1.tar.gz
bmake-20101215.tar.gz
gcc-4.8.3 + os161-2.1.tar.gz
gdb-7.8 + os161-2.1.tar.gz
mk-20100612.tar.gz
os161-base-2.0.3.tar.gz
sys161-2.0.8.tar.gz

Be careful. Two files were changed:
- gdb-7.8+os161-2.1/sim/common/sim-arange.h (deleted line 76: #include "sim-arange.c")
- sys161-2.0.8/include/onel.h (on line 49: added "extern" in extra_selecttime declaration)

The files in os161-downloads are already modified. If you want the original files from the Harvard site, you have to redo the changes.

Execute, in order, from this folder (os161-kit):
attention, the first file (installations with apt-get install) must be run by a user enabled to "sudo"
you must write the password and, if requested, answer yes ("y") a few times

source os161-env-startup.sh
source os161-kit-setup.sh
source os161-kit-build.sh
source os161-kernel-build.sh

attention: after kit-build it is necessary to change the PATH in $HOME/.profile.
I avoided attempting an automatic modification with sed or other tools.
I also recommend that you logout/login (or reboot) before using os161 (possibly before running os161-kernel-build.sh).

In order to install and configure VSCode, please refer to the related PDF document.

G.Cabodi

PS: I thank Francesco Cheinasso and Giuseppe D'Andrea for their collaboration in the development of the scripts and in the identification of changes to gdb and sys161.

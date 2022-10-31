# Progetto di Sistemi Operativi
A cura di Aras Leonardo Kankilic (1888465) e Francesco Saverio Sconocchia Pisoni (1889241) 

## Compilazione
`make` per compilare i programmi top e stuff

## Funzionamento
Top e un task manager che mostra PID, STATO, CPU USAGE(%), MEM USAGE(kb) e nome assegnato a tutti i processi in esecuzione su Linux come l'omonimo programma gia esistente.

Cio' che contraddistingue questo programma e la possibilita di poter inviare istantaneamente segnali suspend, resume, terminate e kill ai processi inserendo il comando e il pid desiderati.

Tramite `help` e possibile consultare la lista dei comandi sopraelencati. Inoltre i processi sono ordinati per utilizzo della cpu, in modo crescente, cosi da identificare velocemente eventuali programmi pesanti in esecuzione da poter terminare.

Per mostrare il corretto funzionamento dei comandi che e possibile mandare, si puo eseguire un programma di prova Stuff ed eseguire successivamente Top per trovare il pid del programma e testare tutti i comandi.



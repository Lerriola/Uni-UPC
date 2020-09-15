#include "CacheSim.h"

/* Posa aqui les teves estructures de dades globals
 * per mantenir la informacio necesaria de la cache
 * */

typedef struct {
	int via0;
	int via1;
	int LRU;
} cache;

cache cach[64];

int acierto;
int fallo;
/* La rutina init_cache es cridada pel programa principal per
 * inicialitzar la cache.
 * La cache es inicialitzada al comen�ar cada un dels tests.
 * */
void init_cache ()
{
    totaltime=0.0;
	/* Escriu aqui el teu codi */
	for (int i = 0; i < 64; i++) {
		cach[i].via0 = -1;
		cach[i].via1 = -1;
		cach[i].LRU = 0;
	}
	acierto = 0;
	fallo = 0;
}

/* La rutina reference es cridada per cada referencia a simular */
void reference (unsigned int address)
{
	unsigned int byte;
	unsigned int bloque_m;
	unsigned int conj_mc;
	unsigned int via_mc;
	unsigned int tag;
	unsigned int miss;	   // boolea que ens indica si es miss
	unsigned int replacement;  // boolea que indica si es reempla�a una linia valida
	unsigned int tag_out;	   // TAG de la linia reempla�ada
	float t1,t2;		// Variables per mesurar el temps (NO modificar)

	t1=GetTime();
	/* Escriu aqui el teu codi */

	byte = address%32;
	bloque_m = address/32;
	conj_mc = bloque_m%64;
	tag = bloque_m/64;
	tag_out = -1;

	via_mc = 0;

	//miss = ((cach[conj_mc].via0 == -1 || cach[conj_mc].via0 != tag) && (cach[conj_mc].via1 == -1 || cach[conj_mc].via1 != tag));
	//replacement = (cach[conj_mc].via0 != -1 && cach[conj_mc].via0 != tag && cach[conj_mc].via1 != -1 && cach[conj_mc].via1 != tag);

	if (cach[conj_mc].via0 == tag){
		acierto++;
		miss = 0;
		replacement = 0;
		via_mc = 0;
		cach[conj_mc].LRU = 1;
	}
	else if (cach[conj_mc].via1 == tag){
		acierto++;
		miss = 0;
		replacement = 0;
		via_mc = 1;
		cach[conj_mc].LRU = 0;
	}

	else if (cach[conj_mc].via0 == -1) {
		fallo++;
		miss = 1;
		replacement = 0;
		cach[conj_mc].via0 = tag;
		cach[conj_mc].LRU = 1;
		via_mc = 0;
	}

	else if (cach[conj_mc].via1 == -1) {
		fallo++;
		miss = 1;
		cach[conj_mc].via1 = tag;
		cach[conj_mc].LRU = 0;
		via_mc = 1;
	}

	else {
		fallo++;
		miss = 1;
		replacement = 1;
		if (cach[conj_mc].LRU == 0) {
			tag_out = cach[conj_mc].via0;
			cach[conj_mc].via0 = tag;
			cach[conj_mc].LRU = 1;
			via_mc = 0;
		}
		else if (cach[conj_mc].LRU == 1) {
			tag_out = cach[conj_mc].via1;
			cach[conj_mc].via1 = tag;
			cach[conj_mc].LRU = 0;
			via_mc = 1;
		}
	}


	/* La funcio test_and_print escriu el resultat de la teva simulacio
	 * per pantalla (si s'escau) i comproba si hi ha algun error
	 * per la referencia actual. Tamb� mesurem el temps d'execuci�
	 * */
	t2=GetTime();
	totaltime+=t2-t1;
	test_and_print2 (address, byte, bloque_m, conj_mc, via_mc, tag,
			miss, replacement, tag_out);
}

/* La rutina final es cridada al final de la simulacio */
void final ()
{
 	/* Escriu aqui el teu codi */
	printf("he hecho %d aciertos", acierto);
	printf("he hecho %d fallos", fallo);

}

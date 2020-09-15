#include "CacheSim.h"

/* Posa aqui les teves estructures de dades globals
 * per mantenir la informacio necesaria de la cache
 * */

int cache[128];

int fallo;
int acierto;

/* La rutina init_cache es cridada pel programa principal per
 * inicialitzar la cache.
 * La cache es inicialitzada al comen�ar cada un dels tests.
 * */
void init_cache ()
{
    totaltime=0.0;
	/* Escriu aqui el teu codi */
	for(int i = 0; i < 128; i++) {
		cache[i] = -1;
	}
	fallo = acierto = 0;
}

/* La rutina reference es cridada per cada referencia a simular */
void reference (unsigned int address)
{
	unsigned int byte;
	unsigned int bloque_m;
	unsigned int linea_mc;
	unsigned int tag;
	unsigned int miss;	   // boolea que ens indica si es miss
	unsigned int replacement;  // boolea que indica si es reempla�a una linia valida
	unsigned int tag_out;	   // TAG de la linia reempla�ada
	float t1,t2;		// Variables per mesurar el temps (NO modificar)

	t1=GetTime();
	/* Escriu aqui el teu codi */

	byte = address%32;
	bloque_m = address/32;
	linea_mc = bloque_m%128;
	tag = bloque_m/128;
	tag_out = 0;

	miss = (cache[linea_mc] == -1 || cache[linea_mc] != tag);
	replacement = (cache[linea_mc] != -1 && cache[linea_mc] != tag);

	if (cache[linea_mc] == -1){
		fallo++;
		cache[linea_mc] = tag;
	}
	else if (replacement){
		fallo++;
		tag_out = cache[linea_mc];
		cache[linea_mc] = tag;
	}
	else acierto++;
	/* La funcio test_and_print escriu el resultat de la teva simulacio
	 * per pantalla (si s'escau) i comproba si hi ha algun error
	 * per la referencia actual. Tamb� mesurem el temps d'execucio
	 * */
	t2=GetTime();
	totaltime+=t2-t1;
	test_and_print (address, byte, bloque_m, linea_mc, tag,
			miss, replacement, tag_out);
}

/* La rutina final es cridada al final de la simulacio */
void final ()
{
 	/* Escriu aqui el teu codi */
	printf("he hecho %d aciertos", acierto);
	printf("he hecho %d fallos", fallo);

}

#include "CacheSim.h"

/* Posa aqui les teves estructures de dades globals
 * per mantenir la informacio necesaria de la cache
 * */

int cache[128];

int aciertos, fallos;

/* La rutina init_cache es cridada pel programa principal per
 * inicialitzar la cache.
 * La cache es inicialitzada al comen�ar cada un dels tests.
 * */
void init_cache ()
{
	/* Escriu aqui el teu codi */
	for (int i = 0; i < 128; i++) {
		cache[i] = -1;
	}
	aciertos = fallos = 0;
}

/* La rutina reference es cridada per cada referencia a simular */
void reference (unsigned int address, unsigned int LE)
{
	unsigned int byte;
	unsigned int bloque_m;
	unsigned int linea_mc;
	unsigned int tag;
	unsigned int miss;
	unsigned int lec_mp;
	unsigned int mida_lec_mp;
	unsigned int esc_mp;
	unsigned int mida_esc_mp;
	unsigned int replacement;
	unsigned int tag_out;

	/* Escriu aqui el teu codi */

	//LE = 1 => escriptura
	//LE = 0 => lectura

	byte = address%32;
	bloque_m = address/32;
	linea_mc = bloque_m%128;
	tag = bloque_m/128;
	tag_out = -1;

	lec_mp = 0;
	mida_esc_mp = 0;
	mida_lec_mp = 0;
	esc_mp = 0;

	miss = 0;
	replacement = (cache[linea_mc] != -1 && cache[linea_mc] != tag);

	if (!LE) {
		//LECTURA
		esc_mp = 0;
		mida_esc_mp=0;

		if (cache[linea_mc] == -1) {
			miss = 1;
			lec_mp = 1;
			mida_lec_mp = 32;
			tag_out = -1;
			cache[linea_mc] = tag;
			fallos++;
		}
		else if (replacement) {
			miss = 1;
			tag_out = cache[linea_mc];
			lec_mp = 1;
			mida_lec_mp = 32;
			cache[linea_mc] = tag;
			fallos++;
		}
		else{
			miss = 0;
			lec_mp = 0;
			mida_lec_mp = 0;
			tag_out = -1;
			aciertos++;
		}
	}else{
		//ESCRITURA

		if (cache[linea_mc] == -1 || cache[linea_mc] != tag) {
			miss = 1;
			fallos++;
			tag_out = -1;
			mida_lec_mp = 0;
			mida_esc_mp = 1;
			esc_mp = 1;
		}
		else {
			miss = 0;
			aciertos++;
			tag_out = -1;
			mida_esc_mp = 1;
			mida_lec_mp = 0;
			esc_mp = 1;
		}
	}

	/* La funcio test_and_print escriu el resultat de la teva simulacio
	 * per pantalla (si s'escau) i comproba si hi ha algun error
	 * per la referencia actual
	 * */
	test_and_print (address, LE, byte, bloque_m, linea_mc, tag,
			miss, lec_mp, mida_lec_mp, esc_mp, mida_esc_mp,
			replacement, tag_out);
}

/* La rutina final es cridada al final de la simulacio */
void final ()
{
 	/* Escriu aqui el teu codi */
	printf("he hecho %d aciertos", aciertos);
	printf("he hecho %d fallos", fallos);
}

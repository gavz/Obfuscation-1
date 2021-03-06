#include "clt_mlm.h"
#include "utils.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

static int
write_setup_params(const struct clt_mlm_state *s, const char *dir, long nu,
                   long size)



{



    char *fname;
    int len;
    mpz_t tmp;
    double start, end;
    start = current_time();

    len = strlen(dir) + 10;

    fname = (char *) malloc(sizeof(char) * len);
    if (fname == NULL)
        return 1;

    mpz_init(tmp);

    // save size
    if (size > 0) {
        mpz_set_ui(tmp, size);
        (void) snprintf(fname, len, "%s/size", dir);
        (void) save_mpz_scalar(fname, tmp);
    }
    // save nu
    mpz_set_ui(tmp, nu);
    (void) snprintf(fname, len, "%s/nu", dir);
    (void) save_mpz_scalar(fname, tmp);
    // save q
    (void) snprintf(fname, len, "%s/q", dir);
    (void) save_mpz_scalar(fname, s->q);
    // save pzt
    (void) snprintf(fname, len, "%s/pzt", dir);
    (void) save_mpz_scalar(fname, s->pzt);

    mpz_clear(tmp);

    free(fname);

    end = current_time();
    if (g_verbose)
        (void) fprintf(stderr, "  Saving to file: %f\n", end - start);

    return 0;
}


 int
write_secret_params(const struct clt_mlm_state *s, const char *dir)

{



    char *fname;
    int len;
    mpz_t tmp;
    double start, end;
    start = current_time();

    len = strlen(dir) + 15;

    fname = (char *) malloc(sizeof(char) * len);
    if (fname == NULL)
        return 1;

    mpz_init(tmp);


//no real reason to go to mpz first, fix this later MR

    // save secparam
        mpz_set_ui(tmp, s->secparam);
        (void) snprintf(fname, len, "%s/secparam.s", dir);
        (void) save_mpz_scalar(fname, tmp);
 
// save rho
        mpz_set_ui(tmp, s->rho);
        (void) snprintf(fname, len, "%s/rho.s", dir);
        (void) save_mpz_scalar(fname, tmp);

// save n
        mpz_set_ui(tmp, s->n);
        (void) snprintf(fname, len, "%s/n.s", dir);
        (void) save_mpz_scalar(fname, tmp);

// save nzs
        mpz_set_ui(tmp, s->nzs);
        (void) snprintf(fname, len, "%s/nzs.s", dir);
        (void) save_mpz_scalar(fname, tmp);


    // save gs
    (void) snprintf(fname, len, "%s/gs.s", dir);
    (void) save_mpz_vector(fname, s->gs,s->n);
    // save crt_coeffs
    (void) snprintf(fname, len, "%s/crt_coeffs.s", dir);
    (void) save_mpz_vector(fname, s->crt_coeffs,s->n);

// save zinvs
    (void) snprintf(fname, len, "%s/zinvs.s", dir);
    (void) save_mpz_vector(fname, s->zinvs,s->nzs);

    mpz_clear(tmp);

    free(fname);

    end = current_time();
    if (g_verbose)
        (void) fprintf(stderr, "  Saving to file: %f\n", end - start);

    return 0;
}


int
delete_secret_params(const char *dir)

{


    char *fname;
    int len;
   
    len = strlen(dir) + 15;

    fname = (char *) malloc(sizeof(char) * len);
    if (fname == NULL)
        return 1;

    
        (void) snprintf(fname, len, "%s/secparam.s", dir);
        (void) unlink(fname);
 
        (void) snprintf(fname, len, "%s/rho.s", dir);
          (void) unlink(fname);

        (void) snprintf(fname, len, "%s/n.s", dir);
        (void) unlink(fname);


        (void) snprintf(fname, len, "%s/nzs.s", dir);
      (void) unlink(fname);


   
    (void) snprintf(fname, len, "%s/gs.s", dir);
   (void) unlink(fname);
    
    (void) snprintf(fname, len, "%s/crt_coeffs.s", dir);
      (void) unlink(fname);


    (void) snprintf(fname, len, "%s/zinvs.s", dir);
   (void) unlink(fname);

    
    free(fname);

  
    return 0;
}

struct clt_mlm_state* load_clt(char* dir_public, char* dir_secret){
	struct clt_mlm_state* s = new struct clt_mlm_state;
	

    char *fname;
    int len;
    mpz_t tmp;
    double start, end;
    start = current_time();

    len = strlen(dir_secret) + 15; //MR fix this later to max length

    fname = (char *) malloc(sizeof(char) * len);
    if (fname == NULL)
        return NULL;

    mpz_init(tmp);


char* dir = dir_secret;

//no real reason to go to mpz first, fix this later MR

    // load secparam
        
        (void) snprintf(fname, len, "%s/secparam.s", dir);
        (void) load_mpz_scalar(fname, tmp);
	s->secparam = mpz_get_ui(tmp);
 
// load rho
        (void) snprintf(fname, len, "%s/rho.s", dir);
        (void) load_mpz_scalar(fname, tmp);
	s->rho= mpz_get_ui(tmp);

// load n
        (void) snprintf(fname, len, "%s/n.s", dir);
        (void) load_mpz_scalar(fname, tmp);
	s->n = mpz_get_ui(tmp);

// load nzs
        
        (void) snprintf(fname, len, "%s/nzs.s", dir);
        (void) load_mpz_scalar(fname, tmp);
	s->nzs = mpz_get_ui(tmp);


    // load gs
	s->gs = new mpz_t[s->n];
	 for (unsigned long i = 0; i < s->n; ++i) {
	mpz_init((s->gs)[i]);
	}
    (void) snprintf(fname, len, "%s/gs.s", dir);
    (void) load_mpz_vector(fname, s->gs,s->n);
    // load crt_coeffs
	s->crt_coeffs = new mpz_t[s->n];
	 for (unsigned long i = 0; i < s->n; ++i) {
	mpz_init((s->crt_coeffs)[i]);
	}
    (void) snprintf(fname, len, "%s/crt_coeffs.s", dir);
    (void) load_mpz_vector(fname, s->crt_coeffs,s->n);

// load zinvs
	s->zinvs = new mpz_t[s->nzs];
	 for (unsigned long i = 0; i < s->nzs; ++i) {
	mpz_init((s->zinvs)[i]);
	}
    (void) snprintf(fname, len, "%s/zinvs.s", dir);
    (void) load_mpz_vector(fname, s->zinvs,s->nzs);

    mpz_clear(tmp);


/// load public stuff
    dir = dir_public;


// load q
    (void) snprintf(fname, len, "%s/q", dir);
    (void) load_mpz_scalar(fname, s->q);
    // load pzt
    (void) snprintf(fname, len, "%s/pzt", dir);
    (void) load_mpz_scalar(fname, s->pzt);


    free(fname);


	seed_rng(&s->rng);
	return s;



}



int
clt_mlm_setup(struct clt_mlm_state *s, const char *dir, const long *pows,
              long kappa, long size, int verbose)
{
    long alpha, beta, eta, nu, rho_f;
    mpz_t *ps, *zs;
    double start, end;

    /* Calculate CLT parameters */
    alpha = s->secparam;
    beta = s->secparam;
    s->rho = s->secparam;
    rho_f = kappa * (s->rho + alpha + 2);
    eta = rho_f + alpha + 2 * beta + s->secparam + 8;
    nu = eta - beta - rho_f - s->secparam + 3;
    s->n = (int) (eta * log2((float) s->secparam));

    if (verbose) {
        fprintf(stderr, "  Security Parameter: %ld\n", s->secparam);
        fprintf(stderr, "  Kappa: %ld\n", kappa);
        fprintf(stderr, "  Alpha: %ld\n", alpha);
        fprintf(stderr, "  Beta: %ld\n", beta);
        fprintf(stderr, "  Eta: %ld\n", eta);
        fprintf(stderr, "  Nu: %ld\n", nu);
        fprintf(stderr, "  Rho: %ld\n", s->rho);
        fprintf(stderr, "  Rho_f: %ld\n", rho_f);
        fprintf(stderr, "  N: %ld\n", s->n);
        fprintf(stderr, "  Number of Zoods: %ld\n", s->nzs);
    }

    ps = (mpz_t *) malloc(sizeof(mpz_t) * s->n);
    s->gs = (mpz_t *) malloc(sizeof(mpz_t) * s->n);
    s->crt_coeffs = (mpz_t *) malloc(sizeof(mpz_t) * s->n);
    zs = (mpz_t *) malloc(sizeof(mpz_t) * s->nzs);
    s->zinvs = (mpz_t *) malloc(sizeof(mpz_t) * s->nzs);

    seed_rng(&s->rng);

    /* initialize gmp variables */
    mpz_init_set_ui(s->q, 1);
    mpz_init_set_ui(s->pzt, 0);
    for (unsigned long i = 0; i < s->n; ++i) {
        mpz_init_set_ui(ps[i], 1);
        mpz_inits(s->gs[i], s->crt_coeffs[i], NULL);
    }
    for (unsigned long i = 0; i < s->nzs; ++i) {
        mpz_inits(zs[i], s->zinvs[i], NULL);
    }

//ProfilerStart("clm.prof");


    /* Generate p_i's and g_i's, as well as q = \prod p_i */
    start = current_time();
mpz_class q;
	q=1;

 #ifdef OpenMP_4
fprintf(stderr, " REDUCING IT! \n");
#pragma omp parallel for reduction(mpz_class_mul_red : q)
    for (unsigned long i = 0; i < s->n; ++i) {
        mpz_t p_unif;
        mpz_init(p_unif);
        // XXX: the primes generated here aren't officially uniform
        mpz_urandomb(p_unif, s->rng, eta);
        mpz_nextprime(ps[i], p_unif);
        mpz_urandomb(p_unif, s->rng, alpha);  //MR - get random primes from sage?
        mpz_nextprime(s->gs[i], p_unif);
		mpz_clear(p_unif);
        q *= mpz_class(ps[i]);
    }
	mpz_set(s->q, q.get_mpz_t());
 #else
#pragma omp parallel for
    for (unsigned long i = 0; i < s->n; ++i) {
        mpz_t p_unif;
        mpz_init(p_unif);
        // XXX: the primes generated here aren't officially uniform
        mpz_urandomb(p_unif, s->rng, eta);
        mpz_nextprime(ps[i], p_unif);
        mpz_urandomb(p_unif, s->rng, alpha);
        mpz_nextprime(s->gs[i], p_unif);
#pragma omp critical
        {
            //
            // This step is very expensive, and unfortunately it blocks the
            // parallelism of generating the primes.
            //
            mpz_mul(s->q, s->q, ps[i]);
        }
        mpz_clear(p_unif);
    }
  

#endif

  end = current_time();

//ProfilerStop();
	
////test 
/*
mpz_class test_q;
test_q =1;
for (unsigned long i = 0; i< s->n; ++i){
		test_q *= mpz_class(ps[i]);
	}
if (mpz_cmp(s->q, test_q.get_mpz_t())==0){
fprintf(stderr, "  It worked!\n");
}else {
fprintf(stderr, " Something's wrong!again!\n");
}*/
/////
    if (g_verbose)
        (void) fprintf(stderr, "  Generating p_i's and g_i's: %f\n",
                       end - start);


    /* Compute CRT coefficients */
    start = current_time();
(void) fprintf(stderr, "Start computing CRT \n");
    //
    // This step is needed for making encoding efficient.  Unfortunately, the
    // CRT coefficients take an enormous amount of memory to compute / store.
    //
#pragma omp parallel for
    for (unsigned long i = 0; i < s->n; ++i) {
        mpz_t q;
        mpz_init(q);
        mpz_tdiv_q(q, s->q, ps[i]);
        mpz_invert(s->crt_coeffs[i], q, ps[i]);
        mpz_mul(s->crt_coeffs[i], s->crt_coeffs[i], q);
        mpz_mod(s->crt_coeffs[i], s->crt_coeffs[i], s->q);
        mpz_clear(q);
    }
    end = current_time();
    if (g_verbose)
        (void) fprintf(stderr, "  Generating CRT coefficients: %f\n",
                       end - start);

    /* Compute z_i's */
    start = current_time();
#pragma omp parallel for
    for (unsigned long i = 0; i < s->nzs; ++i) {
        do {
            mpz_urandomm(zs[i], s->rng, s->q);
        } while (mpz_invert(s->zinvs[i], zs[i], s->q) == 0);
    }
    end = current_time();
    if (g_verbose)
        (void) fprintf(stderr, "  Generating z_i's: %f\n", end - start);

    /* Compute pzt */
    start = current_time();
    {
        mpz_t zk, tmp;
        mpz_init(tmp);
        mpz_init_set_ui(zk, 1);
        // compute z_1^t_1 ... z_k^t_k mod q
        for (unsigned long i = 0; i < s->nzs; ++i) {
            mpz_powm_ui(tmp, zs[i], pows[i], s->q);
	    fprintf(stderr, "Pow %lu : %lu \n ", i, pows[i]);
            mpz_mul(zk, zk, tmp);
            mpz_mod(zk, zk, s->q);
        }

#ifdef OpenMP_4
mpz_class pzt_wrap;
pzt_wrap = 0;
 #pragma omp parallel for reduction(mpz_class_add_red : pzt_wrap)
 for (unsigned long i = 0; i < s->n; ++i) {
            mpz_t tmp, qpi, rnd;
            mpz_inits(tmp, qpi, rnd, NULL);
            // compute (((g_i)^{-1} mod p_i) * z^k mod p_i) * r_i * (q / p_i)
            mpz_invert(tmp, s->gs[i], ps[i]);
            mpz_mul(tmp, tmp, zk);
            mpz_mod(tmp, tmp, ps[i]);
            mpz_genrandom(rnd, &s->rng, beta);
            mpz_mul(tmp, tmp, rnd);
            mpz_div(qpi, s->q, ps[i]);
            mpz_mul(tmp, tmp, qpi);
            mpz_mod(tmp, tmp, s->q);
            pzt_wrap += mpz_class(tmp);
             mpz_clears(tmp, qpi, rnd, NULL);
}
mpz_set(s->pzt,pzt_wrap.get_mpz_t());
mpz_mod(s->pzt, s->pzt, s->q);
        mpz_clear(zk);

#else
#pragma omp parallel for
        for (unsigned long i = 0; i < s->n; ++i) {
            mpz_t tmp, qpi, rnd;
            mpz_inits(tmp, qpi, rnd, NULL);
            // compute (((g_i)^{-1} mod p_i) * z^k mod p_i) * r_i * (q / p_i)
            mpz_invert(tmp, s->gs[i], ps[i]);
            mpz_mul(tmp, tmp, zk);
            mpz_mod(tmp, tmp, ps[i]);
            mpz_genrandom(rnd, &s->rng, beta);
            mpz_mul(tmp, tmp, rnd);
            mpz_div(qpi, s->q, ps[i]);
            mpz_mul(tmp, tmp, qpi);
            mpz_mod(tmp, tmp, s->q);
#pragma omp critical
            {
                mpz_add(s->pzt, s->pzt, tmp);
            }
            mpz_clears(tmp, qpi, rnd, NULL);
        }
        mpz_mod(s->pzt, s->pzt, s->q);
        mpz_clear(zk);
    
#endif

}
    end = current_time();
    if (g_verbose)
        (void) fprintf(stderr, "  Generating pzt: %f\n", end - start);

    (void) write_setup_params(s, dir, nu, size);

    for (unsigned long i = 0; i < s->n; ++i) {
        mpz_clear(ps[i]);
    }
    free(ps);
    for (unsigned long i = 0; i < s->nzs; ++i) {
        mpz_clear(zs[i]);
    }
    free(zs);

    return 0;
}

void
clt_mlm_cleanup(struct clt_mlm_state *s)
{
    gmp_randclear(s->rng);
    mpz_clears(s->q, s->pzt, NULL);
    for (unsigned long i = 0; i < s->n; ++i) {
        mpz_clears(s->gs[i], s->crt_coeffs[i], NULL);
    }
    free(s->gs);
    free(s->crt_coeffs);
    for (unsigned long i = 0; i < s->nzs; ++i) {
        mpz_clear(s->zinvs[i]);
    }
    free(s->zinvs);
}

void
clt_mlm_encode(struct clt_mlm_state *s, mpz_t out, size_t nins,
               const mpz_t *ins, unsigned int nzs, const int *indices,
               const int *pows)
{
   

    mpz_class out_wrap;
    out_wrap = 0;
    mpz_set_ui(out, 0);

#ifdef OpenMP_4
#pragma omp parallel for reduction(mpz_class_add_red : out_wrap)
#else
#pragma omp parallel for
#endif
    for (unsigned long i = 0; i < s->n; ++i) {
        mpz_t r, tmp;
        mpz_inits(r, tmp, NULL);
        mpz_genrandom(r, &s->rng, s->rho);
        mpz_mul(tmp, r, s->gs[i]);
        if (i < nins)
            mpz_add(tmp, tmp, ins[i]);
        mpz_mul(tmp, tmp, s->crt_coeffs[i]);
       out_wrap += mpz_class(tmp);
        mpz_clears(r, tmp, NULL);
    }
    mpz_set(out, out_wrap.get_mpz_t());




    for (unsigned long i = 0; i < nzs; ++i) { //ask how to parallelize. maybe dont mod til end
        mpz_t tmp;
        mpz_init(tmp);
        // Any index set to < 0 means we skip this one
        if (indices[i] < 0)
            continue;
        mpz_powm_ui(tmp, s->zinvs[indices[i]], pows[i], s->q);
        mpz_mul(out, out, tmp);
        mpz_mod(out, out, s->q);
        mpz_clear(tmp);
    }
    
}

int
clt_mlm_is_zero(const mpz_t c, const mpz_t pzt, const mpz_t q, long nu)
{
    mpz_t tmp;
    int ret;

    mpz_init(tmp);
    mpz_mul(tmp, c, pzt);
    mpz_mod(tmp, tmp, q);
    ret = (mpz_sizeinbase(tmp, 2) < (mpz_sizeinbase(q, 2) - nu)) ? 1 : 0;
    mpz_clear(tmp);

    return ret;
}

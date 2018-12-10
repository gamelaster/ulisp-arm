#pragma once

// C Macros

#define nil                NULL
#define car(x)             (((object *) (x))->car)
#define cdr(x)             (((object *) (x))->cdr)

#define first(x)           (((object *) (x))->car)
#define second(x)          (car(cdr(x)))
#define cddr(x)            (cdr(cdr(x)))
#define third(x)           (car(cdr(cdr(x))))

#define push(x, y)         ((y) = cons((x),(y)))
#define pop(y)             ((y) = cdr(y))

#define integerp(x)        ((x) != NULL && (x)->type == NUMBER)
#define floatp(x)          ((x) != NULL && (x)->type == FLOAT)
#define symbolp(x)         ((x) != NULL && (x)->type == SYMBOL)
#define stringp(x)         ((x) != NULL && (x)->type == STRING)
#define characterp(x)      ((x) != NULL && (x)->type == CHARACTER)
#define streamp(x)         ((x) != NULL && (x)->type == STREAM)

#define mark(x)            (car(x) = (object *)(((uintptr_t)(car(x))) | MARKBIT))
#define unmark(x)          (car(x) = (object *)(((uintptr_t)(car(x))) & ~MARKBIT))
#define marked(x)          ((((uintptr_t)(car(x))) & MARKBIT) != 0)
#define MARKBIT            1

#define setflag(x)         (Flags = Flags | 1<<(x))
#define clrflag(x)         (Flags = Flags & ~(1<<(x)))
#define tstflag(x)         (Flags & 1<<(x))

// Constants

const int TRACEMAX = 3; // Number of traced functions
enum type { ZERO=0, SYMBOL=2, NUMBER=4, STREAM=6, CHARACTER=8, FLOAT=10, STRING=12, PAIR=14 };  // STRING and PAIR must be last
enum token { UNUSED, BRA, KET, QUO, DOT };
enum stream { SERIALSTREAM, I2CSTREAM, SPISTREAM, SDSTREAM };

enum function { SYMBOLS, NIL, TEE, NOTHING, AMPREST, LAMBDA, LET, LETSTAR, CLOSURE, SPECIAL_FORMS, QUOTE,
DEFUN, DEFVAR, SETQ, LOOP, PUSH, POP, INCF, DECF, SETF, DOLIST, DOTIMES, TRACE, UNTRACE, FORMILLIS,
WITHSERIAL, WITHI2C, WITHSPI, WITHSDCARD, TAIL_FORMS, PROGN, RETURN, IF, COND, WHEN, UNLESS, AND, OR,
FUNCTIONS, NOT, NULLFN, CONS, ATOM, LISTP, CONSP, SYMBOLP, STREAMP, EQ, CAR, FIRST, CDR, REST, CAAR, CADR,
SECOND, CDAR, CDDR, CAAAR, CAADR, CADAR, CADDR, THIRD, CDAAR, CDADR, CDDAR, CDDDR, LENGTH, LIST, REVERSE,
NTH, ASSOC, MEMBER, APPLY, FUNCALL, APPEND, MAPC, MAPCAR, ADD, SUBTRACT, MULTIPLY, DIVIDE, MOD, ONEPLUS,
ONEMINUS, ABS, RANDOM, MAXFN, MINFN, NOTEQ, NUMEQ, LESS, LESSEQ, GREATER, GREATEREQ, PLUSP, MINUSP, ZEROP,
ODDP, EVENP, INTEGERP, NUMBERP, FLOATFN, FLOATP, SIN, COS, TAN, ASIN, ACOS, ATAN, SINH, COSH, TANH, EXP,
SQRT, LOG, EXPT, CEILING, FLOOR, TRUNCATE, ROUND, CHAR, CHARCODE, CODECHAR, CHARACTERP, STRINGP, STRINGEQ,
STRINGLESS, STRINGGREATER, SORT, STRINGFN, CONCATENATE, SUBSEQ, READFROMSTRING, PRINCTOSTRING,
PRIN1TOSTRING, LOGAND, LOGIOR, LOGXOR, LOGNOT, ASH, LOGBITP, EVAL, GLOBALS, LOCALS, MAKUNBOUND, BREAK,
READ, PRIN1, PRINT, PRINC, TERPRI, READBYTE, READLINE, WRITEBYTE, WRITESTRING, WRITELINE, RESTARTI2C, GC,
ROOM, SAVEIMAGE, LOADIMAGE, CLS, PINMODE, DIGITALREAD, DIGITALWRITE, ANALOGREAD, ANALOGWRITE, DELAY,
MILLIS, SLEEP, NOTE, EDIT, PPRINT, PPRINTALL, ENDFUNCTIONS };

// Typedefs

typedef unsigned int symbol_t;

typedef struct sobject {
  union {
    struct {
      sobject *car;
      sobject *cdr;
    };
    struct {
      unsigned int type;
      union {
        symbol_t name;
        int integer;
        float single_float;
      };
    };
  };
} object;

typedef object *(*fn_ptr_type)(object *, object *);

typedef struct {
  const char *string;
  fn_ptr_type fptr;
  uint8_t min;
  uint8_t max;
} tbl_entry_t;

typedef int (*gfun_t)();
typedef void (*pfun_t)(char);

void error (const char *string);
void error2 (object *symbol, const char *string);
char nthchar (object *string, int n);
void pserial (char c);
void pfl (pfun_t pfun);
void pfstring (const char *s, pfun_t pfun);
inline void pln (pfun_t pfun);
char *lookupsymbol (symbol_t name);
void pint (int i, pfun_t pfun);
uint8_t lookupmin (symbol_t name);
uint8_t lookupmax (symbol_t name);
void testescape ();
int gserial ();
object *read (gfun_t gfun);
void deletesymbol (symbol_t name);
void printstring (object *form, pfun_t pfun);
object *edit (object *fun);
void superprint (object *form, int lm, pfun_t pfun);
int subwidthlist (object *form, int w);
void pstring (char *s, pfun_t pfun);
void supersub (object *form, int lm, int super, pfun_t pfun);

void ulisploop ();
void ulisprepl (object *env);
void ulispsetup ();
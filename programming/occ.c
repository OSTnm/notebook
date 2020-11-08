#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define _(T) || parser->look->tag == T
#define F(C) parser->look->tag == C

#define TYPE_FIRST (F(KW_INT)_(KW_CHAR)_(KW_VOID))
#define STATEMENT_FIRST (F(KW_EXTERN)_(KW_IF)_(KW_ELSE)_(KW_SWITCH)_(KW_CASE)_(KW_DEFAULT)_(KW_WHILE)_(KW_DO)_(KW_FOR)_(KW_BREAK)_(KW_CONTINUE)_(KW_RETURN))

int PARSER_ERRCODE = 0;
#define PARSER_ERR(parser, rc)                                          \
    ({                                                                  \
        int _rc = rc;                                                   \
        if (_rc) {                                                      \
            struct scanner *snr = parser->lexer->snr;                   \
            if (!PARSER_ERRCODE) {                                      \
                printf("parser err: %s<line %d, column %d> \n", snr->name, \
                       snr->line_num, snr->col_num);                    \
            }                                                           \
            return PARSER_ERRCODE = _rc;                                \
        }                                                               \
        PARSER_ERRCODE = 0;                                             \
    })

typedef enum {
    false,
    true,
} bool;

enum lex_errcode{
    STR_NO_R_QUTION,
    NUM_BIN_TYPE,
    NUM_HEX_TYPE,
    CHAR_NO_R_QUTION,
    CHAR_NO_DATA,
    OR_NO_PAIR,
    COMMENT_NO_END,
    TOKEN_NO_EXIST,
};

enum syn_errcode{
    TYPE_LOST,
    TYPE_WRONG,
    ID_LOST,
    ID_WRONG,
    NUM_LOST,
    NUM_WRONG,
    LITERAL_LOST,
    LITERAL_WRONG,
    COMMA_LOST,
    COMMA_WRONG,
    SEMICON_LOST,
    SEMICON_WRONG,
    ASSIGN_LOST,
    ASSIGN_WRONG,
    COLON_LOST,
    COLON_WRONG,
    WHILE_LOST,
    WHILE_WRONG,
    LPAREN_LOST,
    LPAREN_WRONG,
    RPAREN_LOST,
    RPAREN_WRONG,
    LBRACK_LOST,
    LBRACK_WRONG,
    RBRACK_LOST,
    RBRACK_WRONG,
    LBRACE_LOST,
    LBRACE_WRONG,
    RBRACE_LOST,
};

enum sem_errcode{
    VAR_RE_DEF,
    FUN_RE_DEF,
    VAR_UN_DEC,
    FUN_UN_DEC,
    FUN_DEC_ERR,
    FUN_CALL_ERR,
    DEC_INIT_DENY,
    EXTERN_FUN_DEF,
    ARRAY_LEN_INVALID,
    VAR_INIT_ERR,
    GLB_INIT_ERR,
    VOID_VAR,
    EXPR_NOT_LEFT_VAL,
    ASSIGN_TYPE_ERR,
    EXPR_IS_BASE,
    EXPR_NOT_BASE,
    ARR_TYPE_ERR,
    EXPR_IS_VOID,
    BREAK_ERR,
    CONTINUE_ERR,
    RETURN_ERR,
};

enum sem_warn{
    FUN_DEC_CONFLICT,
    FUN_RET_CONFLICT
};

struct entry_s {
	char *key;
	char *value;
	struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
	int size;
	struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;

enum Tag{
    ERR,
    END,
    ID,
    KW_INT,KW_CHAR,KW_VOID,
    KW_EXTERN,
    NUM,CH,STR,
    NOT,LEA,
    ADD,SUB,MUL,DIV,MOD,
    INC,DEC,
    GT,GE,LT,LE,EQU,NEQU,
    AND,OR,
    LPAREN,RPAREN,
    LBRACK,RBRACK,
    LBRACE,RBRACE,
    COMMA,COLON,SEMICON,
    ASSIGN,
    KW_IF,KW_ELSE,
    KW_SWITCH,KW_CASE,KW_DEFAULT,
    KW_WHILE,KW_DO,KW_FOR,
    KW_BREAK,KW_CONTINUE,KW_RETURN,
    COMMENT,
};

struct Token
{
    enum Tag tag;
    void (*delete)(struct Token *token);
    void (*to_str)(struct Token *token);
};

struct Id
{
    struct Token token;
    char *name;
};

struct Num
{
    struct Token token;
    int val;
};

struct Char
{
    struct Token token;
    char ch;
};

struct Str
{
    struct Token token;
    char *str;
};

struct Keywords
{
    hashtable_t *tbl;
};

struct occ_str
{
    char *str;
    char *p;
    int  length;
};

struct scanner
{
#define BUFLEN 80
    FILE *fp;
    char line[BUFLEN];
    char *name;
    int line_len;
    int read_pos;
    int line_num;
    int col_num;
    char last_ch;
};

struct lexer {
    struct scanner *snr;
    struct Token *token;
    int pre_read;
};

struct parser {
    struct lexer *lexer;
    struct Token *look;
};

/* TODO:  */
struct interinst {

};

struct var {
#define SCOPE_DEPTH 5
    bool literal;
    int  scope_patch[SCOPE_DEPTH];
    bool externed;
    enum Tag type;
    char *name;
    bool is_ptr;
    bool is_array;
    int  array_size;
    bool is_left;
    struct var *init_data;
    bool inited;
    union {
        int int_val;
        char char_val;
    };
    char *str_val;
    char *ptr_val;
    struct var *ptr;
    int size;
    int offset;
};

struct fun {
#define FUN_PARA_NUM 10
    bool externed;
    enum Tag type;
    char *name;
    struct var *para_var[FUN_PARA_NUM];
    int max_depth;
    int cur_esp;
    int scope_esp[10];/* TODO:  */
    struct interinst *inter_code[10]; /* TODO:  */
    struct interinst *return_point;
};

struct symtab {
    hashtable_t *var_tbl;
    hashtable_t *str_tbl;
    hashtable_t *fun_tbl;
    int scope_id;
};

int parser_move(struct parser *parser);
int parser_type(struct parser *parser);
int parser_literal(struct parser *parser);
int parser_expr(struct parser *parser);
int parser_altexpr(struct parser *parser);
int parser_arg(struct parser *parser);
int parser_arglist(struct parser *parser);
int parser_realarg(struct parser *parser);
int parser_idexpr(struct parser *parser);
int parser_elem(struct parser *parser);
int parser_rop(struct parser *parser);
int parser_val(struct parser *parser);
int parser_lop(struct parser *parser);
int parser_factor(struct parser *parser);
int parser_muls(struct parser *parser);
int parser_itemtail(struct parser *parser);
int parser_item(struct parser *parser);
int parser_adds(struct parser *parser);
int parser_alotail(struct parser *parser);
int parser_aloexpr(struct parser *parser);
int parser_localdef(struct parser *parser);
int parser_subprogram(struct parser *parser);
int parser_block(struct parser *parser);
int parser_cmps(struct parser *parser);
int parser_cmptail(struct parser *parser);
int parser_cmpexpr(struct parser *parser);
int parser_andtail(struct parser *parser);
int parser_andexpr(struct parser *parser);
int parser_ortail(struct parser *parser);
int parser_orexpr(struct parser *parser);
int parser_asstail(struct parser *parser);
int parser_assexpr(struct parser *parser);
int parser_whilestat(struct parser *parser);
int parser_forinit(struct parser *parser);
int parser_forstat(struct parser *parser);
int parser_dowhilestat(struct parser *parser);
int parser_elsestat(struct parser *parser);
int parser_ifstat(struct parser *parser);
int parser_switchstat(struct parser *parser);
int parser_statement(struct parser *parser);
int parser_paradatatail(struct parser *parser);
int parser_paradata(struct parser *parser);
int parser_paralist(struct parser *parser);
int parser_para(struct parser *parser);
int parser_varrdef(struct parser *parser);
int parser_defdata(struct parser *parser);
int parser_deflist(struct parser *parser);
int parser_funtail(struct parser *parser);
int parser_fun(struct parser *parser);
int parser_idtail(struct parser *parser);
int parser_def(struct parser *parser);
int parser_segment(struct parser *parser);
int parser_program(struct parser *parser);
int parser_init(struct parser *parser);
int parser_program(struct parser *parser);

hashtable_t *ht_create(int size) {

	hashtable_t *hashtable = NULL;
	int i;

	if(size < 1) return NULL;

	/* Allocate the table itself. */
	if((hashtable = malloc(sizeof(*hashtable))) == NULL) {
		return NULL;
	}

	/* Allocate pointers to the head nodes. */
	if((hashtable->table = malloc(sizeof(entry_t *) * size)) == NULL) {
		return NULL;
	}

	for(i = 0; i < size; i++) {
		hashtable->table[i] = NULL;
	}

	hashtable->size = size;
	return hashtable;
}

/* Hash a string for a particular hash table. */
int ht_hash(hashtable_t *hashtable, char *key) {

	unsigned long int hashval = 0;
	int i = 0;

	/* Convert our string to an integer */
	while(hashval < ULONG_MAX && i < strlen(key)) {
		hashval = hashval << 8;
		hashval += key[i];
		i++;
	}

	return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair(char *key, char *value) {
	entry_t *newpair;

	if((newpair = malloc(sizeof(*newpair))) == NULL) {
		return NULL;
	}

    newpair->key = key;
    newpair->value = value;
	newpair->next = NULL;

	return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set(hashtable_t *hashtable, char *key, char *value) {
	int bin = 0;
	entry_t *newpair = NULL;
	entry_t *next = NULL;
	entry_t *last = NULL;

	bin = ht_hash(hashtable, key);

	next = hashtable->table[bin];

	while(next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
		last = next;
		next = next->next;
	}

	/* There's already a pair.  Let's replace that string. */
	if(next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {
		next->value = value;
        /* Nope, could't find it.  Time to grow a pair. */
	} else {
		newpair = ht_newpair(key, value);

		/* We're at the start of the linked list in this bin. */
		if( next == hashtable->table[bin] ) {
			newpair->next = next;
			hashtable->table[bin] = newpair;

            /* We're at the end of the linked list in this bin. */
		} else if (next == NULL) {
			last->next = newpair;
            /* We're in the middle of the list. */
		} else  {
			newpair->next = next;
			last->next = newpair;
		}
	}
}

/* Retrieve a key-value pair from a hash table. */
char *ht_get(hashtable_t *hashtable, char *key) {
	int bin = 0;
	entry_t *pair;

	bin = ht_hash( hashtable, key );

	/* Step through the bin, looking for our value. */
	pair = hashtable->table[bin];
	while(pair != NULL && pair->key != NULL && strcmp(key, pair->key) > 0 ) {
		pair = pair->next;
	}

	/* Did we actually find anything? */
	if(pair == NULL || pair->key == NULL || strcmp(key, pair->key) != 0 ) {
		return NULL;
	} else {
		return pair->value;
	}
}

void token_to_str(struct Token *token)
{
#define TAG_STR(tag) [tag] = #tag
    static char *tags_str[] = {
        TAG_STR(ERR),
        TAG_STR(END),
        TAG_STR(ID),
        TAG_STR(KW_INT),
        TAG_STR(KW_CHAR),
        TAG_STR(KW_VOID),
        TAG_STR(KW_EXTERN),
        TAG_STR(NUM),
        TAG_STR(CH),
        TAG_STR(STR),
        TAG_STR(NOT),
        TAG_STR(LEA),
        TAG_STR(ADD),
        TAG_STR(SUB),
        TAG_STR(MUL),
        TAG_STR(DIV),
        TAG_STR(MOD),
        TAG_STR(INC),
        TAG_STR(DEC),
        TAG_STR(GT),
        TAG_STR(GE),
        TAG_STR(LT),
        TAG_STR(LE),
        TAG_STR(EQU),
        TAG_STR(NEQU),
        TAG_STR(AND),
        TAG_STR(OR),
        TAG_STR(LPAREN),
        TAG_STR(RPAREN),
        TAG_STR(LBRACK),
        TAG_STR(RBRACK),
        TAG_STR(LBRACE),
        TAG_STR(RBRACE),
        TAG_STR(COMMA),
        TAG_STR(COLON),
        TAG_STR(SEMICON),
        TAG_STR(ASSIGN),
        TAG_STR(KW_IF),
        TAG_STR(KW_ELSE),
        TAG_STR(KW_SWITCH),
        TAG_STR(KW_CASE),
        TAG_STR(KW_DEFAULT),
        TAG_STR(KW_WHILE),
        TAG_STR(KW_DO),
        TAG_STR(KW_FOR),
        TAG_STR(KW_BREAK),
        TAG_STR(KW_CONTINUE),
        TAG_STR(KW_RETURN),
        TAG_STR(COMMENT),
    };

    printf("tag: %s\n", tags_str[token->tag]);
    return;
}

void token_delete(struct Token *token)
{
    free(token);
    return;
}

struct Token *token_new()
{
    struct Token *token = malloc(sizeof(*token));

    if (!token) return NULL;
    memset(token, 0, sizeof(*token));
    token->delete = token_delete;
    token->to_str = token_to_str;
    return token;
}

void id_to_str(struct Token *token)
{
    struct Id *id = (struct Id *)token;

    token_to_str(token);
    printf("\\_id: %s\n", id->name);
    return;
}

void id_delete(struct Token *token)
{
    struct Id *id = (struct Id *)token;

    free(id->name);
    free(id);
    return;
}

struct Id *id_new(char *name)
{
    struct Id *id = malloc(sizeof(*id));

    if (!id) return NULL;
    memset(id, 0, sizeof(*id));

    id->token.tag = ID;
    id->token.delete = id_delete;
    id->token.to_str = id_to_str;
    id->name = name;
    return id;
}

void num_to_str(struct Token *token)
{
    struct Num *num = (struct Num *)token;

    token_to_str(token);
    printf("\\_val: %d\n", num->val);
    return;
}

struct Num *num_new(int val)
{
    struct Num *num = malloc(sizeof(*num));

    if (!num) return NULL;
    memset(num, 0, sizeof(*num));

    num->token.tag = NUM;
    num->token.delete = token_delete;
    num->token.to_str = num_to_str;
    num->val = val;
    return num;
}

void char_to_str(struct Token *token)
{
    struct Char *ch = (struct Char *)token;

    token_to_str(token);
    printf("\\_ch: %c\n", ch->ch);
    return;
}

struct Char *char_new(char c)
{
    struct Char *ch = malloc(sizeof(*ch));

    if (!ch) return NULL;
    memset(ch, 0, sizeof(*ch));

    ch->token.tag = CH;
    ch->token.delete = token_delete;
    ch->token.to_str = char_to_str;
    ch->ch = c;
    return ch;
}

void str_delete(struct Token *token)
{
    struct Str *str = (struct Str *)token;

    free(str->str);
    free(str);
    return;
}

void str_to_str(struct Token *token)
{
    struct Str *str = (struct Str *)token;

    token_to_str(token);
    printf("\\_str: %s\n", str->str);
    return;
}

struct Str *str_new(char *s)
{
    struct Str *str = malloc(sizeof(*str));

    if (!str) return NULL;
    memset(str, 0, sizeof(*str));

    str->token.tag = STR;
    str->token.delete = str_delete;
    str->token.to_str = str_to_str;
    str->str = s;
    return str;
}

struct Keywords *keywords_new()
{
    struct Keywords *keywords = malloc(sizeof(*keywords));

    if (!keywords) return NULL;
    memset(keywords, 0, sizeof(*keywords));
    return keywords;
}

void keywords_delete(struct Keywords *keywords)
{
    free(keywords);
    return;
}

int occ_str_init(struct occ_str *str)
{
    memset(str, 0, sizeof(*str));
    str->length = 20;
    str->p = str->str = malloc(str->length);
    memset(str->str, 0, str->length);
    return -!str->str;
}

void occ_str_fini(struct occ_str *str)
{
    free(str);
    memset(str, 0, sizeof(*str));
    return;
}

int occ_str_push(struct occ_str *str, char ch)
{
    if (str->p == (str->str + str->length - 1)) {
        char *tmp = malloc(str->length * 2);
        if (!tmp) return -1;
        memset(tmp, 0, str->length * 2);
        memcpy(tmp, str->str, str->length);
        str->p = tmp + str->length - 1;
        str->length = str->length * 2;
        free(str->str);
        str->str = tmp;
    }

    *str->p++ = ch;
    return 0;
}

#define STR_ERR(err) #err

void lex_error(struct scanner *snr, enum lex_errcode err)
{
    static char *str_err[] = {
        STR_ERR(STR_NO_R_QUTION),
        STR_ERR(NUM_BIN_TYPE),
        STR_ERR(NUM_HEX_TYPE),
        STR_ERR(CHAR_NO_R_QUTION),
        STR_ERR(CHAR_NO_DATA),
        STR_ERR(OR_NO_PAIR),
        STR_ERR(COMMENT_NO_END),
        STR_ERR(TOKEN_NO_EXIST),
    };

    printf("%s<line %d, column %d>:%s\n", snr->name, snr->line_num,
           snr->col_num, str_err[err]);
}

void syn_error(struct scanner *snr, enum syn_errcode err, struct Token *t)
{
    static char *str_err[] = {
        STR_ERR(TYPE_LOST),
        STR_ERR(TYPE_WRONG),
        STR_ERR(ID_LOST),
        STR_ERR(ID_WRONG),
        STR_ERR(NUM_LOST),
        STR_ERR(NUM_WRONG),
        STR_ERR(LITERAL_LOST),
        STR_ERR(LITERAL_WRONG),
        STR_ERR(COMMA_LOST),
        STR_ERR(COMMA_WRONG),
        STR_ERR(SEMICON_LOST),
        STR_ERR(SEMICON_WRONG),
        STR_ERR(ASSIGN_LOST),
        STR_ERR(ASSIGN_WRONG),
        STR_ERR(COLON_LOST),
        STR_ERR(COLON_WRONG),
        STR_ERR(WHILE_LOST),
        STR_ERR(WHILE_WRONG),
        STR_ERR(LPAREN_LOST),
        STR_ERR(LPAREN_WRONG),
        STR_ERR(RPAREN_LOST),
        STR_ERR(RPAREN_WRONG),
        STR_ERR(LBRACK_LOST),
        STR_ERR(LBRACK_WRONG),
        STR_ERR(RBRACK_LOST),
        STR_ERR(RBRACK_WRONG),
        STR_ERR(LBRACE_LOST),
        STR_ERR(LBRACE_WRONG),
        STR_ERR(RBRACE_LOST),
    };

    t->to_str(t);
    if (0 == err % 2) {
        printf("%s<line %d, column %d> lose:%s\n", snr->name, snr->line_num,
               snr->col_num, str_err[err]);
    } else {
        printf("%s<line %d, column %d> not match:%s\n", snr->name,
               snr->line_num, snr->col_num, str_err[err]);
    }

    return;
}

void sem_error(struct scanner *snr, enum sem_errcode err, char *name)
{
    static char *str_err[] = {
        STR_ERR(VAR_RE_DEF),
        STR_ERR(FUN_RE_DEF),
        STR_ERR(VAR_UN_DEC),
        STR_ERR(FUN_UN_DEC),
        STR_ERR(FUN_DEC_ERR),
        STR_ERR(FUN_CALL_ERR),
        STR_ERR(DEC_INIT_DENY),
        STR_ERR(EXTERN_FUN_DEF),
        STR_ERR(ARRAY_LEN_INVALID),
        STR_ERR(VAR_INIT_ERR),
        STR_ERR(GLB_INIT_ERR),
        STR_ERR(VOID_VAR),
        STR_ERR(EXPR_NOT_LEFT_VAL),
        STR_ERR(ASSIGN_TYPE_ERR),
        STR_ERR(EXPR_IS_BASE),
        STR_ERR(EXPR_NOT_BASE),
        STR_ERR(ARR_TYPE_ERR),
        STR_ERR(EXPR_IS_VOID),
        STR_ERR(BREAK_ERR),
        STR_ERR(CONTINUE_ERR),
        STR_ERR(RETURN_ERR),
    };

    printf("%s<line %d, column %d>:%s :%s\n", snr->name, snr->line_num,
           snr->col_num, name, str_err[err]);
}

void sem_warn(struct scanner *snr, enum sem_warn err, char *name)
{
    static char *str_err[] = {
        STR_ERR(FUN_DEC_CONFLICT),
        STR_ERR(FUN_RET_CONFLICT),
    };

    printf("%s<line %d, column %d>:%s :%s\n", snr->name, snr->line_num,
           snr->col_num, name, str_err[err]);
}


#undef STR_ERR

int scanner_init(struct scanner *snr, char *name)
{
    FILE *fp = fopen(name, "r");

    if (!fp) return -1;

    memset(snr, 0, sizeof(*snr));

    snr->name = name;
    snr->fp = fp;
    snr->line_len = 0;
    snr->read_pos = -1;
    snr->line_num = 1;
    snr->col_num = 0;
    snr->last_ch = '\0';
    return 0;
}

char scanner_scan(struct scanner *snr)
{
    char ch;

    if (!snr->fp) return -1;

    if (snr->read_pos == snr->line_len - 1) {
        snr->line_len = fread(snr->line, 1, BUFLEN, snr->fp);
        if (0 == snr->line_len) {
            snr->line_len = 1;
            snr->line[0] = -1;
        }
        snr->read_pos = -1;
    }
    snr->read_pos++;

    ch = snr->line[snr->read_pos];
    if (snr->last_ch == '\n') {
        snr->line_num++;
        snr->col_num = 0;
    }

    if (ch == -1) {
        fclose(snr->fp);
        snr->fp = NULL;
    } else if (ch != '\n') {
        snr->col_num++;
    }

    snr->last_ch = ch;

    return ch;
}

char scanner_scan_need(struct scanner *snr, char need, int *match)
{
    char ch = scanner_scan(snr);

    *match = 0;
    if (need) {
        if (ch != need) {
            *match = -1;
            return ch;
        }
        ch = scanner_scan(snr);
        return ch;
    }

    return ch;
}

char scanner_scan_trim(struct scanner *snr, int pre_read)
{
    char ch;

    if (!pre_read) {
        ch = snr->last_ch;
    } else {
        ch = scanner_scan(snr);
    }

    while (ch == ' ' || ch == '\n' || ch == '\t')
        ch = scanner_scan(snr);

    return ch;
}

int lexer_init(struct lexer *lexer, struct scanner *snr)
{
    memset(lexer, 0, sizeof(*lexer));

    lexer->snr = snr;
    lexer->pre_read = -1;
    return 0;
}

int parser_new(struct parser *parser, struct lexer *lexer)
{
    memset(parser, 0, sizeof(*parser));

    parser->lexer = lexer;
    return 0;
}

int keywords_init(struct Keywords *keywords)
{
    memset(keywords, 0, sizeof(*keywords));
    keywords->tbl = ht_create(31);
    ht_set(keywords->tbl, "int", (char *)KW_INT);
    ht_set(keywords->tbl, "char", (char *)KW_CHAR);
    ht_set(keywords->tbl, "void", (char *)KW_VOID);
    ht_set(keywords->tbl, "extern", (char *)KW_EXTERN);
    ht_set(keywords->tbl, "if", (char *)KW_IF);
    ht_set(keywords->tbl, "else", (char *)KW_ELSE);
    ht_set(keywords->tbl, "switch", (char *)KW_SWITCH);
    ht_set(keywords->tbl, "case", (char *)KW_CASE);
    ht_set(keywords->tbl, "default", (char *)KW_DEFAULT);
    ht_set(keywords->tbl, "while", (char *)KW_WHILE);
    ht_set(keywords->tbl, "do", (char *)KW_DO);
    ht_set(keywords->tbl, "for", (char *)KW_FOR);
    ht_set(keywords->tbl, "break", (char *)KW_BREAK);
    ht_set(keywords->tbl, "continue", (char *)KW_CONTINUE);
    ht_set(keywords->tbl, "return", (char *)KW_RETURN);
    return 0;
}

enum Tag keywords_get_tag(struct Keywords *keywords, char *str)
{
    enum Tag tag = (enum Tag)ht_get(keywords->tbl, str);
    return ERR != tag ? tag : ID;
}

struct Keywords keywords;

struct Token *lexer_tokenize_id(struct lexer *lexer, char ch, int *pre_read)
{
    struct scanner *snr = lexer->snr;
    struct Token *token = NULL;
    struct occ_str str;
    enum Tag tag;

    if (!((ch >= 'a' && ch <= 'z') ||
          (ch >= 'A' && ch <= 'Z') ||
          ch == '_')) {
        return token;
    }

    occ_str_init(&str);
    do
    {
        occ_str_push(&str, ch);
        ch = scanner_scan(snr);
    } while ((ch >= 'a' && ch <= 'z') ||
             (ch >= 'A' && ch <= 'Z') ||
             ch == '_' || (ch >= '0' && ch <= '9'));

    tag = keywords_get_tag(&keywords, str.str);
    if (tag == ID) {
        token = (struct Token *)id_new(str.str);
    } else {
        token = token_new();
        token->tag = tag;
    }

    *pre_read = 0;
    return token;
}

struct Token *lexer_tokenize_num(struct lexer *lexer, char ch, int *pre_read)
{
    struct scanner *snr = lexer->snr;
    struct Token *token = NULL;

    if (!(ch >= '0' && ch <= '9')) return token;

    int val = 0;

    if (ch != '0') {
        do
        {
            val = val * 10 + ch - '0';
            ch = scanner_scan(snr);
        } while (ch >= '0' && ch <= '9');
    } else {
        ch = scanner_scan(snr);
        if (ch == 'x') {
            ch = scanner_scan(snr);
            if ((ch >= '0' && ch <= '9') ||
                (ch >= 'A' && ch <= 'F') ||
                (ch >= 'a' && ch <= 'f')) {
                do
                {
                    val = val * 16 + ch;
                    if (ch >= '0' && ch <= '9') {
                        val -= '0';
                    } else if (ch >= 'A' && ch <= 'F') {
                        val += 10 - 'A';
                    } else if (ch >= 'a' && ch <= 'f') {
                        val += 10 - 'a';
                    }
                    ch = scanner_scan(snr);
                } while (ch >= '0' && ch <= '9' && ch >= 'A' && ch <= 'F');
            } else {
                lex_error(snr, NUM_HEX_TYPE);
                token = token_new();
                token->tag = ERR;
            }
        } else if (ch == 'b') {
            ch = scanner_scan(snr);
            if (ch >= '0' && ch <= '1') {
                do
                {
                    val = val * 2 + ch - '0';
                    ch = scanner_scan(snr);
                } while (ch >= '0' && ch <= '1');
            } else {
                lex_error(snr, NUM_BIN_TYPE);
                token = token_new();
                token->tag = ERR;
            }
        } else if (ch >= '0' && ch <= '7') {
            do
            {
                val = val * 8 + ch - '0';
                ch = scanner_scan(snr);
            } while (ch >= '0' && ch <= '7');
        }
    }

    if (!token) {
        token = (struct Token *)num_new(val);
    }

    *pre_read = 0;
    return token;
}

struct Token *lexer_tokenize_char(struct lexer *lexer, char ch, int *pre_read)
{
    struct scanner *snr = lexer->snr;
    struct Token *token = NULL;
    char c;

    if (!(ch == '\'')) return token;

    ch = scanner_scan(snr);
    if (ch == '\\') {
        ch = scanner_scan(snr);
        if (ch == 'n') c = '\n';
        else if (ch == '\\') c = '\\';
        else if (ch == 't') c = '\t';
        else if (ch == '0') c = '\0';
        else if (ch == '\'') c = '\'';
        else if (ch == -1 || ch == '\n') {
            lex_error(snr, CHAR_NO_R_QUTION);
            token = token_new();
            token->tag = ERR;
        }
        else c = ch;
    } else if (ch == '\n' || ch == -1) {
        lex_error(snr, CHAR_NO_R_QUTION);
        token = token_new();
        token->tag = ERR;
    } else if (ch == '\'') {
        lex_error(snr, CHAR_NO_DATA);
        token = token_new();
        token->tag = ERR;
        ch = scanner_scan(snr);
    } else {
        c = ch;
    }

    if (!token) {
        if ('\'' == (ch = scanner_scan(snr))) {
            token = (struct Token *)char_new(c);
        } else {
            lex_error(snr, CHAR_NO_R_QUTION);
            token = token_new();
            token->tag = ERR;
            *pre_read = 0;
        }
    } else {
        *pre_read = 0;
    }

    return token;
}

struct Token *lexer_tokenize_str(struct lexer *lexer, char ch, int *pre_read)
{
    struct scanner *snr = lexer->snr;
    struct Token *token = NULL;
    struct occ_str str;

    if (ch != '"') return token;

    occ_str_init(&str);
    while ('"' != (ch = scanner_scan(snr))) {
        if (ch == '\\') {
            ch = scanner_scan(snr);
            if (ch == 'n') occ_str_push(&str, '\n');
            else if (ch == '\\') occ_str_push(&str, '\\');
            else if (ch == 't') occ_str_push(&str, '\t');
            else if (ch == '"') occ_str_push(&str, '"');
            else if (ch == '0') occ_str_push(&str, '\0');
            else if (ch == '\n');
            else if (ch == -1) {
                lex_error(snr, STR_NO_R_QUTION);
                token = token_new();
                token->tag = ERR;
                break;
            }
            else occ_str_push(&str, ch);
        } else if (ch == '\n' || ch == -1) {
            lex_error(snr, STR_NO_R_QUTION);
            token = token_new();
            token->tag = ERR;
            break;
        } else {
            occ_str_push(&str, ch);
        }
    }

    if (!token) {
        token = (struct Token *)str_new(str.str);
    } else {
        *pre_read = 0;
    }

    return token;
}

struct Token *lexer_tokenize_bound(struct lexer *lexer, char ch, int *pre_read)
{
    struct scanner *snr = lexer->snr;
    struct Token *token = NULL;

    token = token_new();
    switch (ch) {
    case '+':
        if ('+' == (ch = scanner_scan(snr))) {
            token->tag = INC;
        } else {
            token->tag = ADD;
            *pre_read = 0;
        }
        break;
    case '-':
        if ('-' == (ch = scanner_scan(snr))) {
            token->tag = DEC;
        } else {
            token->tag = SUB;
            *pre_read = 0;
        }
        break;
    case '%':
        token->tag = MOD;
        break;
    case '>': {
        int match;
        ch = scanner_scan_need(snr, '=', &match);
        if (!match) {
            token->tag = GE;
        } else {
            *pre_read = 0;
            token->tag = GT;
        }
        break;
    }
    case '<': {
        int match;
        ch = scanner_scan_need(snr, '=', &match);
        if (!match) {
            token->tag = LE;
        } else {
            *pre_read = 0;
            token->tag = LT;
        }
        break;
    }
    case '=': {
        int match;
        ch = scanner_scan_need(snr, '=', &match);
        if (!match) {
            token->tag = EQU;
        } else {
            *pre_read = 0;
            token->tag = ASSIGN;
        }
        break;
    }
    case '&': {
        int match;
        ch = scanner_scan_need(snr, '&', &match);
        if (!match) {
            token->tag = AND;
        } else {
            *pre_read = 0;
            token->tag = LEA;
        }
        break;
    }
    case '|': {
        int match;
        ch = scanner_scan_need(snr, '|', &match);
        if (!match) {
            token->tag = OR;
        } else {
            *pre_read = 0;
            token->tag = ERR;
        }
        break;
    }
    case '!': {
        int match;
        ch = scanner_scan_need(snr, '=', &match);
        if (!match) {
            token->tag = NEQU;
        } else {
            *pre_read = 0;
            token->tag = NOT;
        }
        break;
    }
    case ',':
        token->tag = COMMA;
        break;
    case ':':
        token->tag = COLON;
        break;
    case ';':
        token->tag = SEMICON;
        break;
    case '(':
        token->tag = LPAREN;
        break;
    case ')':
        token->tag = RPAREN;
        break;
    case '[':
        token->tag = LBRACK;
        break;
    case ']':
        token->tag = RBRACK;
        break;
    case '{':
        token->tag = LBRACE;
        break;
    case '}':
        token->tag = RBRACE;
        break;
    case -1:
        token->tag = END;
        break;
    case '/':
        ch = scanner_scan(snr);
        if (ch == '/') {
            while (!(ch == '\n' || ch == -1)) {
                ch = scanner_scan(snr);
            }
            token->tag = COMMENT;
        } else if (ch == '*') {
            while (-1 != (ch = scanner_scan(snr))) {
                if (ch == '*') {
                    int match = 0;
                    while ((ch = scanner_scan_need(snr ,'*', &match)) && !match);
                    if (ch == '/') {
                        token->tag = COMMENT;
                        break;
                    }
                }
            }

            if (!token && ch == -1) {
                lex_error(snr, COMMENT_NO_END);
                token->tag = ERR;
            }
        } else {
            token->tag = DIV;
        }
        break;
    default:
        token->tag = ERR;
        lex_error(snr, TOKEN_NO_EXIST);
        break;
    }

    return token;
}

struct Token *__lexer_tokenize(struct lexer *lexer, char ch, int *pre_read)
{
    static struct Token *(*funcs[])(struct lexer *, char, int *) = {
        lexer_tokenize_id,
        lexer_tokenize_num,
        lexer_tokenize_char,
        lexer_tokenize_str,
        lexer_tokenize_bound,
    };
    struct Token *token;
    int i;

    for (i = 0; i < ARRAY_SIZE(funcs); ++i) {
        if ((token = funcs[i](lexer, ch, pre_read))) return token;
    }

    return NULL;
}

struct Token *lexer_tokenize(struct lexer *lexer)
{
    struct scanner *snr = lexer->snr;
    enum Tag tag;
    char ch;

    if (lexer->token) {
        tag = lexer->token->tag;
        lexer->token->delete(lexer->token);
        lexer->token = NULL;
    }

    if (END == tag) return NULL;

    ch = scanner_scan_trim(snr, lexer->pre_read);

    lexer->pre_read = -1;
    lexer->token = __lexer_tokenize(lexer, ch, &lexer->pre_read);
    if (!lexer->token) return NULL;
    lexer->token->to_str(lexer->token);
    return lexer->token;
}

int parser_move(struct parser *parser)
{
    do
    {
        parser->look = lexer_tokenize(parser->lexer);
        /* skip comment */
    } while (parser->look && COMMENT == parser->look->tag);

    return parser->look ? 0 : -1;
}

bool parser_match(struct parser *parser, enum Tag need)
{
    if (!parser->look) return false;

    if (parser->look->tag == need) {
        parser_move(parser);
        return true;
    }

    return false;
}

void parser_recovery(struct parser *parser, bool cond,
                     enum syn_errcode err, enum syn_errcode wrong)
{
    if (cond) {
        syn_error(parser->lexer->snr, err, parser->look);
    } else {
        syn_error(parser->lexer->snr, err, parser->look);
        parser_move(parser);
    }
    return;
}

int parser_type(struct parser *parser)
{
    if (!(parser_match(parser, KW_VOID) ||
          parser_match(parser, KW_INT) ||
          parser_match(parser, KW_CHAR))) {
        return -1;
    }

    return 0;
}

int parser_literal(struct parser *parser)
{
    if (!(parser_match(parser, NUM) ||
          parser_match(parser, CH) ||
          parser_match(parser, STR))) {
        return -1;
    }

    return 0;
}

int parser_expr(struct parser *parser)
{
    return parser_assexpr(parser);
}

int parser_altexpr(struct parser *parser)
{
    /* not care return value */
    parser_assexpr(parser);
    return 0;
}

int parser_arg(struct parser *parser)
{
    return parser_expr(parser);
}

int parser_arglist(struct parser *parser)
{
    int rc = 0;

    if (!parser_match(parser, COMMA)) return 0;

    rc = parser_arg(parser);
    PARSER_ERR(parser, rc);
    rc = parser_arglist(parser);
    PARSER_ERR(parser, rc);
    return rc;
}

int parser_realarg(struct parser *parser)
{
    int rc = 0;

    if (!parser_arg(parser)) {
        rc = parser_arglist(parser);
        PARSER_ERR(parser, rc);
    }

    return rc;
}

int parser_idexpr(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, LBRACK)) {
        rc = parser_expr(parser);
        PARSER_ERR(parser, rc);
        rc = -!parser_match(parser, RBRACK);
        PARSER_ERR(parser, rc);
        return rc;
    }

    if (parser_match(parser, LPAREN)) {
        rc = parser_realarg(parser);
        PARSER_ERR(parser, rc);
        rc = -!parser_match(parser, RPAREN);
        PARSER_ERR(parser, rc);
        return rc;
    }

    return rc;
}

int parser_elem(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, ID)) {
        rc = parser_idexpr(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    if (parser_match(parser, LPAREN)) {
        rc = parser_expr(parser);
        PARSER_ERR(parser, rc);
        rc = -!parser_match(parser, RPAREN);
        PARSER_ERR(parser, rc);
        return rc;
    }

    rc = parser_literal(parser);
    PARSER_ERR(parser, rc);

    return rc;
}

int parser_rop(struct parser *parser)
{
    if (!(parser_match(parser, GT) ||
          parser_match(parser, GE))) {
        return -1;
    }

    return 0;
}

int parser_val(struct parser *parser)
{
    int rc = 0;

    rc = parser_elem(parser);
    PARSER_ERR(parser, rc);
    parser_rop(parser);
    return 0;
}

int parser_lop(struct parser *parser)
{
    if (!(parser_match(parser, NOT) ||
          parser_match(parser, SUB) ||
          parser_match(parser, LEA) ||
          parser_match(parser, MUL) ||
          parser_match(parser, INC) ||
          parser_match(parser, DEC))) {
        return -1;
    }

    return 0;
}

int parser_factor(struct parser *parser)
{
    int rc = 0;

    rc = !parser_lop(parser) ? parser_factor(parser) : parser_val(parser);
    PARSER_ERR(parser, rc);
    return rc;
}

int parser_muls(struct parser *parser)
{
    if (!(parser_match(parser, MUL) ||
          parser_match(parser, DIV) ||
          parser_match(parser, MOD))) {
        return -1;
    }

    return 0;
}

int parser_itemtail(struct parser *parser)
{
    int rc = 0;

    if (parser_muls(parser)) {
        return 0;
    }

    rc = parser_factor(parser);
    PARSER_ERR(parser, rc);
    rc = parser_itemtail(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_item(struct parser *parser)
{
    int rc = 0;

    rc = parser_factor(parser);
    PARSER_ERR(parser, rc);
    rc = parser_itemtail(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_adds(struct parser *parser)
{
    if (!(parser_match(parser, ADD) ||
          parser_match(parser, SUB))) {
        return -1;
    }

    return 0;
}

int parser_alotail(struct parser *parser)
{
    int rc = 0;

    if (parser_adds(parser)) {
        return 0;
    }

    rc = parser_item(parser);
    PARSER_ERR(parser, rc);
    rc = parser_alotail(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_aloexpr(struct parser *parser)
{
    int rc = 0;

    rc = parser_item(parser);
    PARSER_ERR(parser, rc);
    rc = parser_alotail(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_localdef(struct parser *parser)
{
    int rc = 0;

    rc = parser_type(parser);
    if (rc) return rc;
    rc = parser_defdata(parser);
    PARSER_ERR(parser, rc);
    rc = parser_deflist(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_subprogram(struct parser *parser)
{
    int rc = 0;

    if (!parser_localdef(parser)) {
        rc = parser_subprogram(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    if (!parser_statement(parser)) {
        rc = parser_subprogram(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    return rc;
}

int parser_block(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, LBRACE)) {
        rc = parser_subprogram(parser);
        PARSER_ERR(parser, rc);
        rc = -!parser_match(parser, RBRACE);
        PARSER_ERR(parser, rc);
    }

    return rc;
}

int parser_cmps(struct parser *parser)
{
    if (!(parser_match(parser, GT) ||
          parser_match(parser, GE) ||
          parser_match(parser, LT) ||
          parser_match(parser, LE) ||
          parser_match(parser, EQU) ||
          parser_match(parser, NEQU))) {
        return -1;
    }

    return 0;
}

int parser_cmptail(struct parser *parser)
{
    int rc;

    rc = parser_cmps(parser);
    if (rc) return 0;

    rc = parser_aloexpr(parser);
    PARSER_ERR(parser, rc);
    rc = parser_cmptail(parser);
    PARSER_ERR(parser, rc);
    return rc;
}

int parser_cmpexpr(struct parser *parser)
{
    int rc = 0;

    rc = parser_aloexpr(parser);
    PARSER_ERR(parser, rc);
    rc = parser_cmptail(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_andtail(struct parser *parser)
{
    int rc = 0;

    if (!parser_match(parser, AND)) {
        return rc;
    }

    rc = parser_cmpexpr(parser);
    PARSER_ERR(parser, rc);
    rc = parser_andtail(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_andexpr(struct parser *parser)
{
    int rc = 0;

    rc = parser_cmpexpr(parser);
    PARSER_ERR(parser, rc);
    rc = parser_andtail(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_ortail(struct parser *parser)
{
    if (!parser_match(parser, OR)) {
        return 0;
    }

    parser_andexpr(parser);
    parser_ortail(parser);
    return 0;
}

int parser_orexpr(struct parser *parser)
{
    int rc = 0;

    rc = parser_andexpr(parser);
    PARSER_ERR(parser, rc);
    rc = parser_ortail(parser);
    PARSER_ERR(parser, rc);
    return rc;
}

int parser_asstail(struct parser *parser)
{
    int rc = 0;

    if (!parser_match(parser, ASSIGN)) {
        return rc;
    }

    rc = parser_orexpr(parser);
    PARSER_ERR(parser, rc);
    rc = parser_asstail(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_assexpr(struct parser *parser)
{
    int rc = 0;

    rc = parser_orexpr(parser);
    PARSER_ERR(parser, rc);
    rc = parser_asstail(parser);
    PARSER_ERR(parser, rc);
    return rc;
}

int parser_whilestat(struct parser *parser)
{
    int rc = 0;

    rc = -!parser_match(parser, KW_WHILE);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, LPAREN);
    PARSER_ERR(parser, rc);
    rc = parser_altexpr(parser);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, RPAREN);
    PARSER_ERR(parser, rc);
    rc = parser_block(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_forinit(struct parser *parser)
{
    int rc = 0;

    if (!parser_localdef(parser)) {
        return rc;
    }

    rc = parser_altexpr(parser);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, SEMICON);
    PARSER_ERR(parser, rc);
    return rc;
}

int parser_forstat(struct parser *parser)
{
    int rc = 0;

    rc = -!parser_match(parser, KW_FOR);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, LPAREN);
    PARSER_ERR(parser, rc);
    rc = parser_forinit(parser);
    PARSER_ERR(parser, rc);
    rc = parser_altexpr(parser);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, SEMICON);
    PARSER_ERR(parser, rc);
    rc = parser_altexpr(parser);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, RPAREN);
    PARSER_ERR(parser, rc);
    rc = parser_block(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_dowhilestat(struct parser *parser)
{
    int rc = 0;

    rc = -!parser_match(parser, KW_DO);
    PARSER_ERR(parser, rc);
    rc = parser_block(parser);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, KW_WHILE);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, LPAREN);
    PARSER_ERR(parser, rc);
    rc = parser_altexpr(parser);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, RPAREN);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, SEMICON);
    PARSER_ERR(parser, rc);
    return rc;
}

int parser_elsestat(struct parser *parser)
{
    int rc = 0;

    if (!parser_match(parser, KW_ELSE)) {
        return 0;
    }

    rc = parser_block(parser);
    PARSER_ERR(parser, rc);
    return rc;
}

int parser_ifstat(struct parser *parser)
{
    int rc = 0;

    rc = -!parser_match(parser, KW_IF);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, LPAREN);
    PARSER_ERR(parser, rc);
    rc = parser_expr(parser);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, RPAREN);
    PARSER_ERR(parser, rc);
    rc = parser_block(parser);
    PARSER_ERR(parser, rc);
    rc = parser_elsestat(parser);
    PARSER_ERR(parser, rc);
    return rc;
}

int parser_caselabel(struct parser *parser)
{
    return parser_literal(parser);
}

int parser_casestat(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, KW_CASE)) {
        rc = parser_caselabel(parser);
        PARSER_ERR(parser, rc);
        rc = -!parser_match(parser, COLON);
        PARSER_ERR(parser, rc);
        rc = parser_subprogram(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    if (parser_match(parser, KW_DEFAULT)) {
        rc = -!parser_match(parser, COLON);
        PARSER_ERR(parser, rc);
        rc = parser_subprogram(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    PARSER_ERR(parser, rc = -1);
    return rc;
}

int parser_switchstat(struct parser *parser)
{
    int rc = 0;

    rc = -!parser_match(parser, KW_SWITCH);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, LPAREN);
    PARSER_ERR(parser, rc);
    rc = parser_expr(parser);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, RPAREN);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, LBRACE);
    PARSER_ERR(parser, rc);
    rc = parser_casestat(parser);
    PARSER_ERR(parser, rc);
    rc = -!parser_match(parser, RBRACE);
    PARSER_ERR(parser, rc);
    return rc;
}


int parser_statement(struct parser *parser)
{
    int rc = 0;

    switch (parser->look->tag) {
    case KW_WHILE:
        rc = parser_whilestat(parser);
        break;
    case KW_FOR:
        rc = parser_forstat(parser);
        break;
    case KW_DO:
        rc = parser_dowhilestat(parser);
        break;
    case KW_IF:
        rc = parser_ifstat(parser);
        break;
    case KW_SWITCH:
        rc = parser_switchstat(parser);
        break;
    case KW_BREAK:
    case KW_CONTINUE:
        rc = parser_move(parser);
        PARSER_ERR(parser, rc);
        rc = -!parser_match(parser, SEMICON);
        PARSER_ERR(parser, rc);
        break;
    case KW_RETURN:
        rc = parser_move(parser);
        PARSER_ERR(parser, rc);
        rc = parser_altexpr(parser);
        PARSER_ERR(parser, rc);
        rc = -!parser_match(parser, SEMICON);
        PARSER_ERR(parser, rc);
        break;
    case SEMICON:
        rc = parser_move(parser);
        PARSER_ERR(parser, rc);
        break;
    case RPAREN:
    case RBRACK:
    case RBRACE:
        rc = -1;
        break;
    default:
        rc = parser_altexpr(parser);
        PARSER_ERR(parser, rc);
        rc = -!parser_match(parser, SEMICON);
        PARSER_ERR(parser, rc);
        break;
    }

    return rc;
}

int parser_init(struct parser *parser)
{
    if (parser_match(parser, ASSIGN)) {
        parser_expr(parser);
    }

    return 0;
}

int parser_paradatatail(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, LBRACK)) {
        rc = -!parser_match(parser, NUM);
        PARSER_ERR(parser, rc);
        rc = -!parser_match(parser, RBRACK);
        PARSER_ERR(parser, rc);
    }

    return rc;
}

int parser_paradata(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, MUL)) {
        rc = -!parser_match(parser, ID);
        PARSER_ERR(parser, rc);
        return rc;
    }

    if (parser_match(parser, ID)) {
        return parser_paradatatail(parser);
    }

    PARSER_ERR(parser, rc = -1);
    return rc;
}

int parser_paralist(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, COMMA)) {
        rc = parser_type(parser);
        PARSER_ERR(parser, rc);
        rc = parser_paradata(parser);
        PARSER_ERR(parser, rc);
        rc = parser_paralist(parser);
        PARSER_ERR(parser, rc);
    }

    return 0;
}

int parser_para(struct parser *parser)
{
    if (!parser_type(parser)) {
        parser_paradata(parser);
        parser_paralist(parser);
    }

    return 0;
}

int parser_varrdef(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, LBRACK)) {
        rc = -!parser_match(parser, NUM);
        PARSER_ERR(parser, rc);
        rc = -!parser_match(parser, RBRACK);
        PARSER_ERR(parser, rc);
        return rc;
    }

    return parser_init(parser);
}

int parser_defdata(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, ID)) {
        rc = parser_varrdef(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    if (parser_match(parser, MUL)) {
        if (!parser_match(parser, ID)) {
            PARSER_ERR(parser, rc = -1);
        }
        rc = parser_init(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    PARSER_ERR(parser, rc = -1);
    return rc;
}

int parser_deflist(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, COMMA)) {
        rc = parser_defdata(parser);
        PARSER_ERR(parser, rc);
        rc = parser_deflist(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    if (parser_match(parser, SEMICON)) {
        return rc;
    }

    PARSER_ERR(parser, rc = -1);
    return rc;
}

int parser_funtail(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, SEMICON)) {
        return rc;
    }

    return parser_block(parser);
}

int parser_fun(struct parser *parser)
{
    int rc = 0;

    rc = -!parser_match(parser, ID);
    PARSER_ERR(parser, rc);

    rc = -!parser_match(parser, LPAREN);
    PARSER_ERR(parser, rc);

    rc = parser_para(parser);
    PARSER_ERR(parser, rc);

    rc = -!parser_match(parser, RPAREN);
    PARSER_ERR(parser, rc);

    rc = parser_funtail(parser);
    PARSER_ERR(parser, rc);

    return 0;
}

int parser_idtail(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, LPAREN)) {
        rc = parser_para(parser);
        PARSER_ERR(parser, rc);
        rc = -!parser_match(parser, RPAREN);
        PARSER_ERR(parser, rc);
        rc = parser_funtail(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    if (!parser_varrdef(parser)) {
        rc = parser_deflist(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    PARSER_ERR(parser, rc = -1);
    return rc;
}

int parser_def(struct parser *parser)
{
    int rc = 0;

    if (parser_match(parser, ID)) {
        rc = parser_idtail(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    if (parser_match(parser, MUL)) {
        rc = -!parser_match(parser, ID);
        PARSER_ERR(parser, rc);
        rc = parser_init(parser);
        PARSER_ERR(parser, rc);
        rc = parser_deflist(parser);
        PARSER_ERR(parser, rc);
        return rc;
    }

    PARSER_ERR(parser, rc = -1);
    return rc;
}

int parser_segment(struct parser *parser)
{
    int rc = 0;

    /* declaration or definition*/
    parser_match(parser, KW_EXTERN);

    rc = parser_type(parser);
    PARSER_ERR(parser, rc);

    rc = parser_def(parser);
    PARSER_ERR(parser, rc);

    return 0;
}

int parser_program(struct parser *parser)
{
    int rc = 0;

    if (parser->look && parser->look->tag == END) {
        return rc;
    }

    rc = parser_segment(parser);
    PARSER_ERR(parser, rc);
    rc = parser_program(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int parser_analyze(struct parser *parser)
{
    int rc = 0;

    rc = parser_move(parser);
    PARSER_ERR(parser, rc);
    rc = parser_program(parser);
    PARSER_ERR(parser, rc);
    return 0;
}

int main(int argc, char *argv[])
{
    int rc = 0;
    struct scanner snr;
    struct lexer lexer;
    struct parser parser;

    if (2 != argc) return rc;

    if ((rc = scanner_init(&snr, argv[1]))) {
        return rc;
    }

    if ((rc = lexer_init(&lexer, &snr))) {
        return rc;
    }

    if ((rc = parser_new(&parser, &lexer))) {
        return rc;
    }

    keywords_init(&keywords);

    parser_analyze(&parser);
    /* while (lexer_tokenize(&lexer)); */

    return 0;
}

#include <stdio.h>
#include <string.h>

enum {INP_STR_SIZE = 82, VAR_NUM = 32, NAME_LEN = 33, MAX_STR_LEN = 10001};

struct var {
    char name[NAME_LEN];
    char val[INP_STR_SIZE];
};

char * find_substitution(char str[], struct var variables[], int var_num,
    int * succ, int * var_ind) {
    char * first_subs = NULL;
    *succ = 0;
    for (int i = 0; i < var_num; i++) {
        char *p = strstr(str, variables[i].name);
        if (p != NULL) {
            if (p != str && *(p - 1) == '$') {
                if  (first_subs == NULL) {
                    first_subs = p - 1;
                    *succ = 1;
                    *var_ind = i;
                }
                else {
                    if ((p - 1) < first_subs) {
                        first_subs = p - 1;
                        *var_ind = i;
                    }
                    if ((p - 1) == first_subs &&
                    strlen(variables[i].name) > strlen(variables[*var_ind].name)) {
                        first_subs = p - 1;
                        *var_ind = i;
                    }
                }
            }
        }
    }
    return first_subs;
}

void substitute(char * dest, struct var src) {
    int curr_len = strlen(src.name) + 1;
    int next_len = strlen(src.val);
    int delta = next_len - curr_len;
    memmove(dest + curr_len + delta, dest + curr_len, strlen(dest + curr_len) + 1);
    memcpy(dest, src.val, strlen(src.val));
}

int check_var(char s[]) {
    int i = 0;
    while (('a' <= s[i] && s[i] <= 'z') || ('A' <= s[i] && s[i] <= 'Z')) {
        i++;
    }
    if (s[i] == '=' && i != 0) {
        return 1;
    }
    return 0;
}

int is_variable(char s[], struct var vars[], int var_num) {
    int ans = 0, i = 0;
    while (('a' <= s[i] && s[i] <= 'z') || ('A' <= s[i] && s[i] <= 'Z')) {
        i++;
    }
    if (s[i] == '=' && i != 0) {
        ans = 1;
        char name[NAME_LEN];
        memcpy(name, s, i);
        *(name + i) = '\0';
        for (int j = 0; j < var_num; j++) {
            if (strcmp(name, vars[j].name) == 0) {
                memcpy(vars[j].val, s + i + 1, strlen(s) - i - 1);
                *(vars[j].val + strlen(s) - i - 1) = '\0';
                return ans;
            }
        }
        memcpy(vars[var_num].name, s, i);
        *(vars[var_num].name + i) = '\0';
        memcpy(vars[var_num].val, s + i + 1, strlen(s) - i - 1);
        *(vars[var_num].val + strlen(s) - i - 1) = '\0';
    }
    return ans;
}

int
main(void) {
    char inp_str[INP_STR_SIZE];
    char *inp_p;
    struct var variables[VAR_NUM];
    static char tmp[MAX_STR_LEN];
    int var_num = 0;
    char * subs_p;
    int succ, var_ind;

    inp_p = fgets(inp_str, INP_STR_SIZE, stdin);
    while (inp_p != NULL) {
        while (*inp_p != '\n') {
            inp_p++;
        }
        *inp_p = '\0';
        strcpy(tmp, inp_str);

        subs_p = find_substitution(tmp, variables, var_num, &succ, &var_ind);
        while (!(tmp[0] == '#' || check_var(tmp) || succ == 0)) {
            substitute(subs_p, variables[var_ind]);
            subs_p = find_substitution(tmp, variables, var_num, &succ, &var_ind);
        }
        if (tmp[0] != '#') {
            if (is_variable(tmp, variables, var_num)) {
                var_num++;
            }
            else {
                printf("%s\n", tmp);
            }
        }
        inp_p = fgets(inp_str, INP_STR_SIZE, stdin);
    }
}

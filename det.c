#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int count_s(const char *s, const char *ss) {
	int sz1 = strlen(s);
	int sz2 = strlen(ss);
	if (sz1 < sz2) return 0;
	if (sz1 < 2*sz2) return (!strncmp(s, ss, sz2));
	int r=0, i, j;
	for(i=0; i<sz1; i++) {
		if (s[i]==ss[0]) {
			for (j=0; j<sz2; j++) {
				if (s[i+j]!=ss[j]) {
					r--;
					break;
				}
			}
			r++;
			i+=sz2-1;
		}
	}
	return r;
}

char **split_s(const char *s, const char *ss) {
        int c = count_s(s, ss), sz = strlen(s), ssz = strlen(ss);
        if (c==0) return NULL;
        char **r = (char**) calloc(c+2, sizeof(char*));
        int i, j=0;
        for (i=0; i<c+1; i++) r[i]=(char*) calloc(sz, sizeof(char));
        for (i=0; i<sz; i++) {
                if (!strncmp(s+i, ss, ssz) && i!=0) {
                        j++;
						i+=ssz-1;
                        continue;
                }
                r[j][strlen(r[j])]=s[i];
        }
        return r;
}

typedef struct {
	int lines;
	int columns;
	float **mat;
} matrice;

matrice scan_file(const char* path) {
	FILE* stream = fopen(path, "r");
	char buffer[1024];
	memset(buffer, 0, 1024);
	fread(buffer, 1024, 1, stream);
	fclose(stream);
	int n=1, p=1, i=0, j;
	while (buffer[i]!='\n') {
		if (buffer[i]==' ') p++;
		i++;
	}
	i=0;
	while (buffer[i]!='\0') {
		if (buffer[i]=='\n') n++;
		i++;
	}
	float **mat = (float**) calloc(n, sizeof(float*));
	for (i=0; i<p; i++) mat[i] = (float*) calloc(p, sizeof(float));
	char **lines = split_s(buffer, "\n");
	for (i=0; i<n; i++) {
		char **col = split_s(lines[i], " ");
		for (j=0; j<p; j++) mat[i][j] = atof(col[j]);
	}
	matrice *m = (matrice*) malloc(sizeof(matrice));
	m->lines = n;
	m->columns = p;
	m->mat = mat;
	return *m;
}

matrice delta_n(matrice m, int n) {
	matrice *d = (matrice*) malloc(sizeof(matrice));
	int i, j;
	d->lines = m.lines-1;
	d->columns = m.columns-1;
	float **T = (float**) calloc(d->lines, sizeof(float*));
	for (i=0; i<d->columns; i++) T[i] = (float*) calloc(d->columns, sizeof(float));
	for (i=0; i<d->lines+1; i++) {
		if (i==n) continue;
		for (j=0; j<d->columns; j++) {
			if (i<n) T[i][j] = m.mat[i][j+1];
			else T[i-1][j] = m.mat[i][j+1];
		}
	}
	d->mat = T;
	return *d;
}

float determ(matrice M) {
	if (M.lines == 1) return M.mat[0][0];
	float d=0;
	for (int i=0; i<M.lines; i++) {
		d+=pow(-1, i) * M.mat[i][0] * determ(delta_n(M, i));
	}
	return d;
}

int main(int argc, char const *argv[]) {
	matrice m = scan_file(argv[1]);
	printf("%d lines %d columns\n", m.lines, m.columns);
	printf("det = %f\n", determ(m));
	return 0;
}
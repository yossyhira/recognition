#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 最小値を求める関数
int min(int a, int b, int c) {
    int min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}

// レーベンシュタイン距離と操作を計算する関数
void levenshteinDistance(char *str1, char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int **matrix = (int **)malloc((len1 + 1) * sizeof(int *));
    for (int i = 0; i <= len1; i++) {
        matrix[i] = (int *)malloc((len2 + 1) * sizeof(int));
    }

    // 初期化
    for (int i = 0; i <= len1; i++) matrix[i][0] = i;
    for (int j = 0; j <= len2; j++) matrix[0][j] = j;

    // レーベンシュタイン距離の計算
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
            matrix[i][j] = min(matrix[i - 1][j] + 1,         // 削除
                               matrix[i][j - 1] + 1,         // 挿入
                               matrix[i - 1][j - 1] + cost); // 置換
        }
    }

    // 距離の結果を取得
    int i = len1;
    int j = len2;
    while (i > 0 && j > 0) {
        int score = matrix[i][j];
        int scoreDiag = matrix[i - 1][j - 1];
        int scoreUp = matrix[i][j - 1];
        int scoreLeft = matrix[i - 1][j];
        if (score == scoreDiag && str1[i - 1] == str2[j - 1]) {
            i--;
            j--;
        } else if (score == scoreLeft + 1) {
            printf("削除誤り: '%c' を位置 %d から削除\n", str1[i - 1], i);
            i--;
        } else if (score == scoreUp + 1) {
            printf("挿入(付加)誤り: '%c' を位置 %d に挿入\n", str2[j - 1], j);
            j--;
        } else if (score == scoreDiag + 1) {
            printf("置換誤り: '%c' (位置 %d) を '%c' に置換\n", str1[i - 1], i, str2[j - 1]);
            i--;
            j--;
        }
    }

    // 残りの削除と挿入を処理
    while (i > 0) {
        printf("削除誤り: '%c' を位置 %d から削除\n", str1[i - 1], i);
        i--;
    }
    while (j > 0) {
        printf("挿入(付加)誤り: '%c' を位置 %d に挿入\n", str2[j - 1], j);
        j--;
    }

    // 2次元配列のメモリを解放
    for (int k = 0; k <= len1; k++) {
        free(matrix[k]);
    }
    free(matrix);
}

int main() {
    char str1[100], str2[100];

    // ユーザーからの入力を受け取る
    printf("入力１: ");
    scanf("%s", str1);
    printf("入力２: ");
    scanf("%s", str2);

    // レーベンシュタイン距離と操作を計算して出力
    levenshteinDistance(str1, str2);

    return 0;
}

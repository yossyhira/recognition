#include <stdio.h>
#include <string.h>

// 文字列の比較を行い、誤りの種類を判別する関数
void detectErrorType(char *str1, char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int diff = len1 - len2;
    int true_letter;
    int diff_letter;

    // 文字列の長さが同じ場合、置換誤りの可能性がある
    if (diff == 0) {
        int count = 0;
        for (int i = 0; i < len1; i++) {
            if (str1[i] != str2[i]) {
                count++;
                true_letter=diff_letter=i;  
            }
        }
        if (count == 1) {
            printf("置換誤り(%d文字目の'%c' を '%c' に置換する)\n",diff_letter+1, str2[diff_letter], str1[true_letter]);
        } else {
            printf("複数の誤り\n");
        }
    }
    // 文字列2が文字列1よりも長い場合、挿入誤りの可能性がある
    else if (diff == -1) {
        int i = 0, j = 0;
        while (i < len1 && j < len2) {
            if (str1[i] != str2[j]) {
                    printf("挿入(付加)誤り(%d文字目の'%c'を削除する)\n",j+1,str2[j]);
                    return;
            } else {
                i++;
                j++;
            }
        }
        printf("挿入(付加)誤り(%d文字目の'%c'を削除する)\n",j+1,str2[j]);
    }
    // 文字列2が文字列1よりも短い場合、削除誤りの可能性がある
    else if (diff == 1) {
        int i = 0, j = 0;
        while (i < len1 && j < len2) {
            if (str1[i] != str2[j]) {
                    printf("削除誤り(%d文字目に'%c'を挿入する)\n",i+1,str1[i]);
                    return;
            } else {
                i++;
                j++;
            }
        }
        printf("削除誤り(%d文字目に'%c'を挿入する)\n",i+1,str1[i]);
    } else {
        printf("複数の誤り\n");
    }
}

int main() {
    char str1[100], str2[100];

    // ユーザーからの入力を受け取る
    printf("入力１: ");
    scanf("%s", str1);
    printf("入力２: ");
    scanf("%s", str2);

    printf("操作: ");
   // 誤りの種類を検出して出力
    detectErrorType(str1, str2);

    return 0;
}

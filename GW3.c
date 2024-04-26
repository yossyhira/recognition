#include <stdio.h>
#include <string.h>

// 文字列の比較を行い、誤りの種類を判別する関数
void OptimalString(char *str1, char *str2) {

    while(1){
        
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
                if(count==1){
                    break;
                }  
            }
        }
        if (count == 1) {
            printf("置換誤り(%d文字目の'%c' を '%c' に置換する)\n",diff_letter+1, str2[diff_letter], str1[true_letter]);
            str2[diff_letter]=str1[true_letter];
        } else if(count==0){
            printf("最適な文字列に操作後：%s\n",str2);
            break;
        }
    }

    
    // 文字列2が文字列1よりも長い場合、挿入誤りの可能性がある
    else if (diff <= -1) {
        int i = 0, j = 0;
        while (i < len1 && j < len2) {
            if (str1[i] != str2[j]) {
                    printf("挿入(付加)誤り(%d文字目の'%c'を削除する)\n",j+1,str2[j]);
                   for (int k = j; k < len2 - 1; k++) {
                     str2[k] = str2[k + 1];
                   }
                   str2[len2 - 1] = '\0';
                   break;
            } else {
                i++;
                j++;
            }
        }
    }
    // 文字列2が文字列1よりも短い場合、削除誤りの可能性がある
    else if (diff >= 1) {
        int i = 0, j = 0;
        while (i < len1 && j < len2) {
            if (str1[i] != str2[j]) {
                    printf("削除誤り(%d文字目に'%c'を挿入する)\n",i+1,str1[i]);
                   for (int k = len2; k > j; k--) {
                     str2[k] = str2[k - 1];
                    }
                   str2[j]=str1[i];
                   str2[len2 + 1] = '\0';
                   break; 
            } else {
                i++;
                j++;
              }
        }
       
    } 
    }

    
}

int main() {
    char str1[100], str2[100];

    printf("入力１: ");
    scanf("%s", str1);
    printf("入力２: ");
    scanf("%s", str2);

    OptimalString(str1, str2);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXSIZE 150
#define DIMENTION 15
#define ALLFILE 100

typedef struct {
    char name[20];
    char word[20];
    int flame;
    double melcepstrum[MAXSIZE][DIMENTION];
} file_data;

int getCityNum(int a){
    switch (a) {
    case 1: return 11; break;
    case 2:return 12;break;
    case 3:return 21;break;
    case 4:return 22;break;
    default:printf("無効な選択です。\n");break;
    }
};

int main(void) {
    int tmp,unknown;
    FILE *fp_tmp, *fp_unknown;
    file_data city_tmp, city_unknown;
    char ch[200];
    double d[MAXSIZE][MAXSIZE];
    double g[MAXSIZE][MAXSIZE];
    double T[ALLFILE];
    double T_min;
    int match_Idx = 0;
    int count = 0;

    printf("テンプレートの音声データを以下から番号で選択してください。\n 1.city011 2.city012 3.city021 4.city022 : "); scanf("%d",&tmp);
    tmp=getCityNum(tmp);

    printf("未知の音声データを以下から番号で選択してください。\n 1.city011 2.city012 3.city021 4.city022 : "); scanf("%d",&unknown);
    unknown=getCityNum(unknown);

    printf("\nテンプレートの音声データ : city%03d\n未知の音声データ : city%03d\nこの2つのデータを音声認識をさせます。\n\n", tmp, unknown);

    for (int tmp_Idx = 0; tmp_Idx < ALLFILE; tmp_Idx++) {
        sprintf(ch, "/home/hirata/recognition/city_mcepdata/city%.3d/city%.3d_%.3d.txt", tmp, tmp, tmp_Idx + 1);
       
        //ファイルを読み込みモードで開く
        fp_tmp = fopen(ch, "r");
        
        //ファイル読み取りエラー処理
        if (fp_tmp == NULL) {
            printf("テンプレートの音声データが開けませんでした。\n");
            exit(EXIT_FAILURE);
        }
        
        fgets(city_tmp.name, sizeof(city_tmp.name), fp_tmp);//ファイル名取得
        fgets(city_tmp.word, sizeof(city_tmp.word), fp_tmp);//単語取得
        fscanf(fp_tmp, "%d", &city_tmp.flame);//フレーム数取得

        for (int i = 0; i < city_tmp.flame; i++) {
            for (int j = 0; j < DIMENTION; j++) {
                fscanf(fp_tmp, "%lf", &city_tmp.melcepstrum[i][j]);//メルケプストラム特徴量取得
            }
        }
        fclose(fp_tmp);

        for (int unknown_Idx = 0; unknown_Idx < ALLFILE; unknown_Idx++) {

            sprintf(ch, "/home/hirata/recognition/city_mcepdata/city%.3d/city%.3d_%.3d.txt", unknown, unknown, unknown_Idx + 1);
            
            //ファイルを読み込みモードで開く
            fp_unknown = fopen(ch, "r");
            
            //ファイル読み取りエラー処理
            if (fp_unknown == NULL) {
                printf("未知の音声データが開けませんでした。\n");
                exit(EXIT_FAILURE);
            }

            fgets(city_unknown.name, sizeof(city_unknown.name), fp_unknown);//ファイル名取得
            fgets(city_unknown.word, sizeof(city_unknown.word), fp_unknown);//単語取得
            fscanf(fp_unknown, "%d", &city_unknown.flame);//フレーム数取得
           
            for (int i = 0; i < city_unknown.flame; i++) {
                for (int j = 0; j < DIMENTION; j++) {
                    fscanf(fp_unknown, "%lf", &city_unknown.melcepstrum[i][j]);//メルケプストラム特徴量取得
                }
            }
            fclose(fp_unknown);

            //デフォルトの距離を生成　ユークリッド距離
            for (int i = 0; i < city_tmp.flame; i++) {
                for (int j = 0; j < city_unknown.flame; j++) {
                    d[i][j] = 0;
                    for (int k = 0; k < DIMENTION; k++) {
                        d[i][j] += (city_tmp.melcepstrum[i][k] - city_unknown.melcepstrum[j][k]) * (city_tmp.melcepstrum[i][k] - city_unknown.melcepstrum[j][k]);
                    }
                    d[i][j] = sqrtl(d[i][j]);
                }
            }

            //最短距離を計算
            g[0][0] = d[0][0];//スタートの場所
            for (int i = 1; i < city_tmp.flame; i++) {
                g[i][0] = g[i - 1][0] + d[i][0];
            }//1列目の最適ルート
            for (int j = 1; j < city_unknown.flame; j++) {
                g[0][j] = g[0][j - 1] + d[0][j];
            }//1行目の最適ルート
            for (int i = 1; i < city_tmp.flame; i++) {
                for (int j = 1; j < city_unknown.flame; j++) {
                    double a = g[i][j - 1] + d[i][j];
                    double b = g[i - 1][j - 1] + 2 * d[i][j];
                    double c = g[i - 1][j] + d[i][j];
                    g[i][j] = fmin(a, fmin(b, c));
                }
            }//残りの行列の最短ルート

            T[unknown_Idx] = g[city_tmp.flame - 1][city_unknown.flame - 1] / (city_tmp.flame + city_unknown.flame);//単語間距離
            T_min = (unknown_Idx == 0) ? T[unknown_Idx] : fmin(T_min, T[unknown_Idx]);//最小の単語間距離
            match_Idx = (T_min == T[unknown_Idx]) ? unknown_Idx : match_Idx;//最小の単語間距離の未知の単語のファイル番号(インデックス)
        }

        if (match_Idx == tmp_Idx) {
            count++;//一致していれば次に進む
        } else {
            printf("以下のテンプレートの音声データと未知の音声データが一致しているという誤認識をしました。\n");
            printf("テンプレートの音声データ : city%03d/city%03d_%03d.txt\n", tmp, tmp, tmp_Idx + 1);
            printf("未知の音声データ : city%03d/city%03d_%03d.txt\n\n", unknown, unknown, match_Idx + 1);
        }
    }
    printf("認識率 %d%% です。\n", count);
    return 0;
}

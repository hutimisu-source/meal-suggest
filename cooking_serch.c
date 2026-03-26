#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<ctype.h>

typedef struct dishes {
	int		i_Id;
	char	str_DishName[64];
	int		i_CookingTime;
	int		i_IngNum;
	int		i_IngId[32];
	char	str_Url[512];
}gl_st_DishInfo;

gl_st_DishInfo gl_st_DI[16];


// 文字列が数字だけかをチェックする関数(input_cand,serch_dish内で使用)
int is_number_string(const char* str) {
	for (int i = 0; i < (int)strlen(str); i++) {
		if (!isdigit((unsigned char)str[i])) return 0;  // 数字以外があれば 0 を返す
	}
	return 1;  // 全て数字なら 1 を返す
}



int input_cand(int b_Foods[]) {

	FILE* fp;

	

	int i_Num, i_UserInput, i_Loop;
	char str_Name[64];

	fopen_s(&fp, "ingredients.txt", "r");

	if (fp == NULL) {
		return -101;
	}
	else {


		fscanf_s(fp, "%d", &i_Loop);

		if (i_Loop != 32) return -102;		
	


		for (int i = 0;i < i_Loop;i++) {
			fscanf_s(fp, "%d%s", &i_Num, str_Name,(int)sizeof(str_Name));

			
			if (i_Num < 0 || i_Loop <= i_Num) return -103;				//食材番号が範囲外になっていないか確認
			else if (is_number_string(str_Name)) return -104;			//食材名のデータに数字が紛れ込んでいないか確認

			printf("(%d) %s", i_Num, str_Name);

		}
		printf("\n");
	}

	fclose(fp);

	printf("使用する料理に対応する食材を選んでください\n");
	printf("100が入力されると終了します\n");


	while (1) {
		scanf_s("%d", &i_UserInput);
		if (i_UserInput == 100)break;
		else if (i_UserInput < 0 || i_Loop < i_UserInput) return -200;		//選択肢にない数値の入力		
		else b_Foods[i_UserInput] = 1;
	}

	return 0;
}

int serch_dish(int b_Foods[],int i_DishNum[]) {

	FILE* fp;

	int i_Loop, i_Id;

	fopen_s(&fp, "cooking.txt", "r");

	
	
	if (fp == NULL) {
		return -101;
	}
	else {

		printf("ファイルのオープンに成功しました。\n");

		fscanf_s(fp, "%d", &i_Loop);

		if (i_Loop != 16) {
			
			return -106;			//料理の種類数が適切か確認
		}

		int i_Before = -1;

		for (int i = 0;i < i_Loop;i++) {
			fscanf_s(fp, "%d", &gl_st_DI[i].i_Id);
			if (gl_st_DI[i].i_Id < 0 || i_Loop <= gl_st_DI[i].i_Id) return -103;					//料理番号が範囲内か確認
			fscanf_s(fp, "%s", gl_st_DI[i].str_DishName, (int)sizeof(gl_st_DI[i].str_DishName));
			if (is_number_string(gl_st_DI[i].str_DishName)) return -104;							//食材名のデータに数字が紛れ込んでいないか確認
	//		else if (i_Before + 1 != gl_st_DI[i].i_Id)return-109;
			fscanf_s(fp, "%d", &gl_st_DI[i].i_CookingTime);
			fscanf_s(fp, "%d", &gl_st_DI[i].i_IngNum);

			for (int j = 0;j < gl_st_DI[i].i_IngNum;j++) {
				fscanf_s(fp, "%d", &i_Id);
				gl_st_DI[i].i_IngId[j] = i_Id;
			}
			fscanf_s(fp, "%s", gl_st_DI[i].str_Url, (int)sizeof(gl_st_DI[i].str_Url));

			for (int k = 0;k < gl_st_DI[i].i_IngNum;k++) {
				if (b_Foods[gl_st_DI[i].i_IngId[k]] != 1)break;
				if (k == gl_st_DI[i].i_IngNum - 1)i_DishNum[i] = 1;
			}

		}

	}

	fclose(fp);

	return 0;

}

int print_result(int i_DishNum[]) {


	FILE* fp;


	for (int i = 0;i < 16;i++) {
		int i_ncnt = 0;
		if (i_DishNum[i] == 1) {

			fopen_s(&fp, "ingredients.txt", "r");


			if (fp == NULL) {
				printf("ファイルの読み込みに失敗しました。\n");
				return -101;
			}
			else {

				printf("ファイルのオープンに成功しました。\n");
				printf("%s\n", gl_st_DI[i].str_DishName);
				printf("%d分\n", gl_st_DI[i].i_CookingTime);
				printf("使用する食材\n");



				int		i_Loop, i_Num, i_A = 0;
				char	str_Name[64];

				fscanf_s(fp, "%d", &i_Loop);
				if (i_Loop != 32) {
					printf("ingredients.txt内で食材数が見つかりません\n");
					return -102;
				}



				for (int j = 0;j < i_Loop;j++) {
					fscanf_s(fp, "%d%s", &i_Num, str_Name, (int)sizeof(str_Name));
					if (i_Num < 0 || i_Loop <= i_Num) return -103;				//食材番号が範囲外になっていないか確認
					else if (is_number_string(str_Name)) return -104;			//食材名のデータに数字が紛れ込んでいないか確認

					if (i_Num == gl_st_DI[i].i_IngId[i_A]) {
						printf("・%s\n", str_Name);
						i_A++;
						if (i_A == gl_st_DI[i].i_IngNum)break;
					}
				}

				printf("詳細はこちら↓\n");
				printf("%s\n", gl_st_DI[i].str_Url);
			}
		}
		else {
			i_ncnt++;
			if (i_ncnt == 16)printf("入力された材料で提示できる料理の候補がありません\n");
		}
	}

	return 0;
}


/*

int main(void) {

	int i_Foods[32], i_DishNum[16];

	input_cand(i_Foods);

	serch_dish(i_Foods,i_DishNum);

	print_result(i_DishNum);


	return 0;
}


*/


int main(void) {

	int i_Foods[32], i_DishNum[16];

	int i_Code;

	i_Code = input_cand(i_Foods);
	if (i_Code != 0) {
		printf("関数input_candで異常検知\n");

		if (i_Code == -101)printf("ファイルingredients.txtがありません\n");
		else if (i_Code == -102)printf("ingredients.txt内で食材数が見つかりません\n");
		else if (i_Code == -103)printf("ingredients.txt内で対応する食材番号がありません\n");
		else if (i_Code == -104)printf("ingredients.txt内で食材番号が数値になっています\n");
		else if (i_Code == -200)printf("入力された数値は選択肢にありません\n");

		return 0;
	}


	i_Code = serch_dish(i_Foods, i_DishNum);
	if (i_Code != 0) {
		printf("関数serch_dishで異常検知\n");

		if (i_Code == -101)printf("ファイルcooking.txtがありません\n");
		else if (i_Code == -103)printf("cooking.txt内で対応する料理番号がありません\n");
		else if (i_Code == -106)printf("cooking.txt内で料理番号が数値になっています\n");

		return 0;
	}


	i_Code=print_result(i_DishNum);
	if (i_Code != 0) {
		printf("関数print_resultで異常検知\n");

		if (i_Code == -101)printf("ファイルingredients.txtがありません\n");
		else if (i_Code == -102)printf("ingredients.txt内で食材数が見つかりません\n");
		else if (i_Code == -103)printf("ingredients.txt内で対応する食材番号がありません\n");
		else if (i_Code == -104)printf("ingredients.txt内で食材番号が数値になっています\n");

		return 0;
	}

	return 0;
}

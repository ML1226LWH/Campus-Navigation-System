#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>

#define MAX_NUM 500
#define INFINITY 9999999
char admin_name[10] = { 'a','d','m','i','n' };
char admin_pwd[10] = { '1','2','3','4','5','6' };
int place_delete = -1;
/*�û���¼��Ϣ*/
typedef struct account {
	char user_name[10];
	char pwd[10];
}admin;

/*������Ϣ*/
typedef struct place {
	int num;
	char name[20];
	char intro[400];
}vertextype;

/* ͼ�ṹ��Ϣ */
typedef struct mgraph
{
	vertextype vexs[MAX_NUM];//װ�ؾ����š����֡���Ϣ
	int edge[MAX_NUM][MAX_NUM];//װ��·��
	int vexNum, edgNum;//ͼ�ж������ͱ���
}mgraphtype;

mgraphtype* g;

void initSystem();//��ʼ��ҳ��
int click_place(int);//���ѡ�񾰵�
void admin_UI();//����Ա����
void visitor_UI();//�οͽ���
void find_short();//��ѯ��������·��
void Shortpath_Print(int, int);//չʾ��������·��
void find_static();//��ѯ�����̶������·����Ϣ
void show_static(int, int[]);//չʾ�����̶������·����Ϣ
void adminChange();//����Ա��¼�����
void admin_Place_Change();//�޸ľ�����Ϣ
void admin_Place_delete();//ɾ��������Ϣ
void pwd_Change();//����Ա�޸�����
void non_edge_rectangle(int,int,int,int);//���ޱߵľ���
void outputtext(int, int, char*);//����������ܳ���ҳ�������
void Create_Map(mgraphtype*);//У԰����ͼ�Ķ�ȡ�ʹ���
void Floyd(mgraphtype*);//�Ͻ�˹�����㷨�����·��
int Floyd_Print(mgraphtype*, int, int, int);///*�ݹ�ʵ�ִ�ӡ���������·��*/

/*У԰����ͼ�Ķ�ȡ�ʹ���*/
void Create_Map(mgraphtype* g)
{
	int i, j, k, e;
	FILE* rf;
	rf = fopen("res/map.txt", "r");
	if (rf)
	{
		fscanf(rf, "%d %d", &g->vexNum, &g->edgNum);
		for (i = 0; i < g->vexNum; i++)
		{
			fscanf(rf, "\n%d %s\n\n%s", &g->vexs[i].num, g->vexs[i].name, g->vexs[i].intro);
			//printf("%d %s %s", g->vexs[i].num, g->vexs[i].name, g->vexs[i].intro);
		}
		for (i = 0; i < g->vexNum; i++)
			for (j = 0; j < g->vexNum; j++)
			{
				if (i == j)
					g->edge[i][j] = 0;
				else
					g->edge[i][j] = INFINITY;
			}
		for (k = 0; k < g->edgNum; k++)
		{
			fscanf(rf, "%d%d%d", &i, &j, &e);
			g->edge[i][j] = g->edge[j][i] = e;
		}
		fclose(rf);
	}
	else
		g->edgNum = 0;
}

/*�Ͻ�˹�����㷨�����·��*/
int dist[MAX_NUM][MAX_NUM], path[MAX_NUM][MAX_NUM];
void Floyd(mgraphtype* g)
{
	int i, j, k, min, u;
	for (i = 0; i < g->vexNum; i++)
	{
		for (j = 0; j < g->vexNum; j++)
		{
			dist[i][j] = INFINITY;
		}
	}
	for (i = 0; i < g->vexNum; i++)
	{
		for (j = 0; j < g->vexNum; j++)
		{
			dist[i][j] = g->edge[i][j];
			if (i != j && dist[i][j] < INFINITY)
			{
				path[i][j] = i;
			}
			else
			{
				path[i][j] = -1;
			}
		}
	}
	/*��Թ���Աɾ������Ĳ���*/
	if (place_delete >= 0)
	{
		for (i = 0; i < g->vexNum; i++)
		{
			dist[i][place_delete] = INFINITY;
			dist[place_delete][i] = INFINITY;
			path[i][j] = -1;
		}
	}
	for (k = 0; k < g->vexNum; k++)
	{
		for (i = 0; i < g->vexNum; i++)
		{
			for (j = 0; j < g->vexNum; j++)
			{
				if (dist[i][j] > dist[i][k] + dist[k][j]&&path[i][k]!=-1&&path[k][j]!=-1) {
					dist[i][j] = dist[i][k] + dist[k][j];
					path[i][j] = k;
				}
			}
		}
	}
	
}

/*�ݹ�ʵ�ִ�ӡ���������·��*/
int Floyd_Print(mgraphtype* g, int sNum, int eNum,int path_y) {

	if (path[sNum][eNum] == -1 || path[sNum][eNum] == eNum || path[sNum][eNum] == sNum)
	{
		return path_y;
	}
	else {
		path_y=Floyd_Print(g, sNum, path[sNum][eNum],path_y);  /*���м����Ϊ�յ������ӡ·��*/
		outtextxy(475, path_y, _T(g->vexs[path[sNum][eNum]].name));/*��ӡ�м侰������*/
		path_y += 30;
		path_y=Floyd_Print(g, path[sNum][eNum], eNum,path_y); /*���м����Ϊ��������ӡ·��*/
		
	}
	return path_y;
}

/*�������ӡ���������·��*/
void Shortpath_Print(int sNum,int eNum) 
{
	MOUSEMSG m2;
	int path_y = 280;
	outtextxy(475, 40, _T(g->vexs[--sNum].name));
	outtextxy(475, 80, _T("��"));
	outtextxy(475, 120, _T(g->vexs[--eNum].name));
	outtextxy(475, 160, _T("��̾����ǣ�"));
	char num[20];
	sprintf(num, "%d", dist[sNum][eNum]);
	outtextxy(475, 200, _T(num));
	outtextxy(475, 240, _T("�����������·�ߣ�"));
	settextstyle(20, 0, _T("����"));//�����ֺš�����
	outtextxy(475, path_y, _T(g->vexs[sNum].name));            /*���·���ϵ����*/
	path_y += 30;
	path_y=Floyd_Print(g, sNum, eNum,path_y);                 /*���·���ϵ��м��*/
	outtextxy(475, path_y, _T(g->vexs[eNum].name));          /*���·���ϵ��յ�*/ 
	outtextxy(770, 500, _T("����"));
	while (1)
	{
		m2 = GetMouseMsg();
		/*ȷ����*/
		if (m2.x > 770 && m2.x < 960 && m2.y>500 && m2.y < 560 && m2.uMsg == WM_LBUTTONDOWN)
		{initSystem();}
	}
}

/*�������·��*/
void find_short()
{
	MOUSEMSG m;
	settextstyle(30, 0, _T("����"));
	outtextxy(575, 110, _T("��ѡ�����"));
	int place_start = 0;
	place_start = click_place(0);
	outtextxy(575, 180, _T(g->vexs[place_start - 1].name));
	outtextxy(575, 310, _T("��ѡ���յ�"));
	int place_end = 0;
	place_end = click_place(0);
	outtextxy(575, 400, _T(g->vexs[place_end - 1].name));
	outtextxy(770, 500, _T("ȷ��"));
	while (1)
	{
		m = GetMouseMsg();
		/*ȷ����*/
		if (m.x > 770 && m.x < 960 && m.y>500 && m.y < 560 && m.uMsg == WM_LBUTTONDOWN)
		{
			non_edge_rectangle(467, 0, 1000, 600);
			Shortpath_Print(place_start, place_end);
			break;
		}
	}
}

/*���Ҿ��̶������·����Ϣ*/
void find_static()
{
	MOUSEMSG m2;
	settextstyle(30, 0, _T("����"));//�����ֺš�����
	outtextxy(575, 25, _T("��ѡ����������"));
	outtextxy(575, 55, _T("������������յ���ѡ����"));
	outtextxy(555, 85, _T("���ɳ���7����"));
	int place_static[10];
	int i = 0;
	int place_static_y=125;
	int flag = 0;
	char num[20];
	while (i < 7)
	{
		flag= click_place(1);
		if (flag == 999&&i>1)
		{
			break;
		}
		else if (flag == 999 && i <= 1)
		{
			continue;
		}
		else
		{
			place_static[i] = flag;
			outtextxy(575, place_static_y, _T(g->vexs[place_static[i] - 1].name));
			place_static_y += 30;
			i++;
		}
		if (i > 1)
		{
			outtextxy(770, 500, _T("ȷ��"));
		}
	}
	if (i > 6)
	{
		settextstyle(30, 0, _T("����"));
		outtextxy(575, 400, _T("��ѡ���㲻�ɳ���7��!"));
		Sleep(1500);
	}
	non_edge_rectangle(467, 0, 1000, 600);
	show_static(i, place_static);
}

/*չʾ���̶������·����Ϣ*/
void show_static(int len,int place_static[])
{
	MOUSEMSG m2;
	settextstyle(20, 0, _T("����"));
	outtextxy(475, 25, _T("����ѯ�����������·��Ϊ��"));
	int static_y = 55;
	int distance = 0;
	
	for (int i = 0;place_static[i]>0&&place_static[i+1]>0&&i<len; i++)
	{
		outtextxy(475, static_y, _T(g->vexs[place_static[i] - 1].name));
		static_y += 30;
		static_y =Floyd_Print(g,place_static[i]-1,place_static[i+1]-1, static_y);
		distance += dist[place_static[i] - 1][place_static[i + 1] - 1];//�ο�����ĵ�i����i+1������ľ���
	}
	outtextxy(475, static_y, _T(g->vexs[place_static[len - 1]-1].name));
	static_y += 30;
	outtextxy(675, 100, _T("ȫ���ܳ�Ϊ��"));
	char dist[30];
	sprintf(dist, "%d", distance);
	outtextxy(775,100, _T(dist));
	outtextxy(770, 500, _T("����"));
	while (1)
	{
		m2 = GetMouseMsg();
		/*ȷ����*/
		if (m2.x > 770 && m2.x < 960 && m2.y>500 && m2.y < 560 && m2.uMsg == WM_LBUTTONDOWN)
		{
			initSystem();
		}
	}
}

/*���ޱߵľ���*/
void non_edge_rectangle(int left, int up, int right, int down)
{
	setcolor(RGB(255, 255, 255));
	fillrectangle(left, up, right, down);
	setfillcolor(RGB(255, 255, 255));
	setcolor(RGB(6, 67, 6));
	return;
}
/*�οͽ���*/
void visitor_UI()
{
	MOUSEMSG m,m2;
	setcolor(RGB(255, 255, 255));
	settextcolor(RGB(6, 67, 6));
	settextstyle(30, 0, _T("����"));//�����ֺš�����
	outtextxy(575, 25, _T("У԰����ϵͳ"));
	outtextxy(575, 110, _T("��ѡ��Ҫִ�еĲ���"));
	settextstyle(20, 0, _T("����"));//�����ֺš�����
	outtextxy(575, 230, _T("��ѯ��������·��"));
	outtextxy(575, 330, _T("��ѯ�����̶���������·��"));
	outtextxy(575, 430, _T("��ѯ������Ϣ"));

	while (1)
	{
		
		m = GetMouseMsg();
		/*��ѯ��������·��*/
		if (m.x > 575 && m.x < 750 && m.y>230 && m.y < 265 && m.uMsg == WM_LBUTTONDOWN)
		{
			non_edge_rectangle(467, 0, 1000, 600);
			find_short();
			break;
		}
		/*��ѯ�����̶���������·��*/
		else if (m.x > 575 && m.x < 750 && m.y>330 && m.y < 365 && m.uMsg == WM_LBUTTONDOWN)
		{
			non_edge_rectangle(467, 0, 1000, 600);
			find_static();
			break;
		}
		/*��ѯ������Ϣ*/
		else if (m.x > 575 && m.x < 750 && m.y>430 && m.y < 465 && m.uMsg == WM_LBUTTONDOWN)
		{
			non_edge_rectangle(467, 0, 1000, 600);
			outtextxy(575, 110, _T("����Ҫ��ѯ�ĵص�"));
			int place = click_place(0);
			outtextxy(575, 150, _T(g->vexs[place - 1].name));
			outputtext(575, 190, _T(g->vexs[place - 1].intro));
			
			settextstyle(40,0,_T("����"));
			outtextxy(770, 500, _T("����"));
			FlushMouseMsgBuffer();
			while (1)
			{
				m2 = GetMouseMsg();
				/*ȷ����*/
				if (m2.x > 770 && m2.x < 960 && m2.y>500 && m2.y < 560 && m2.uMsg == WM_LBUTTONDOWN)
				{
					initSystem();
				}
			}
			break;
		}
	}
	return;
}

/*����Ա����*/
void admin_UI()
{
	MOUSEMSG m;
	/*ȷ����*/
	setcolor(RGB(6, 67, 6));

	outtextxy(690, 375, _T("ȷ��"));
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, _T("����"));//�����ֺš�����
	outtextxy(400, 20, _T("����Ա��¼"));
	outtextxy(260, 185, _T("�û�����"));

	line(380, 225, 630, 225);
	outtextxy(260, 285, _T("���룺"));
	line(340, 320, 630, 320);

	int flag_name = 0, flag_pwd = 0;
	int charx_name = 380, chary_name = 185;
	int charx_pwd = 340, chary_pwd = 285;
	char name[15];
	char pwd[15];
	while (1)
	{
		/*�����û���*/
		m = GetMouseMsg();
		if (m.x > 260 && m.x < 630 && m.y>185 && m.y < 225 && m.uMsg == WM_LBUTTONDOWN && flag_name <= 9)
		{
			InputBox(name, 10,"�������û���",NULL, "�������û���",300,200);
			//getkb_string(charx_name, chary_name,name,flag_name);
			non_edge_rectangle(373, 182, 770, 219);
			outtextxy(charx_name,chary_name,_T(name));
			charx_name = 380, chary_name = 185;
		}
		/*��������*/
		else if (m.x > 260 && m.x < 630 && m.y>285 && m.y < 320 && m.uMsg == WM_LBUTTONDOWN)
		{
			InputBox(pwd, 10, "����������", NULL, "����������", 300, 200);
			non_edge_rectangle(334, 270, 838, 307);//����ԭʼ��Ϣ
			//getkb_string(charx_pwd, chary_pwd, pwd, flag_pwd);
			flag_pwd = strlen(pwd);
			for (int i = 0; i < flag_pwd; i++)
			{
				outtextxy(charx_pwd, chary_pwd, _T("*"));
				charx_pwd += 20;
			}
			charx_pwd = 340, chary_pwd = 285;
		}
		else if (m.x > 660 && m.x < 860 && m.y>365 && m.y < 425 && m.uMsg == WM_LBUTTONDOWN)
		if (strcmp(name, admin_name) == 0 && strcmp(pwd, admin_pwd) == 0)
		{
			non_edge_rectangle(0, 0, 1000, 600);
			adminChange();
		}
		else
		{
			settextcolor(RGB(248, 81, 55));
			outtextxy(248, 396, _T("������û������󣬼���������ҳ��"));
			settextcolor(RGB(6, 67, 6));
			Sleep(1500);
			initSystem();
		}
	}
}


void adminChange()
{
	MOUSEMSG m;
	settextcolor(RGB(6, 67, 6));
	outtextxy(304, 25, _T("��ѡ��Ҫ���еĲ���"));
	outtextxy(338, 133, _T("1���޸�����"));
	outtextxy(304, 225, _T("2��ɾ��У԰��ͼ����"));
	outtextxy(304, 315, _T("3���޸�У԰��ͼ����"));
	outtextxy(770, 500, _T("����"));
	while (1)
	{
		m = GetMouseMsg();
		/*ȷ����*/
		if (m.x > 770 && m.x < 960 && m.y>500 && m.y < 560 && m.uMsg == WM_LBUTTONDOWN)
		{
			initSystem();
		}
		else if (m.x > 337 && m.x < 510 && m.y>132 && m.y < 174 && m.uMsg == WM_LBUTTONDOWN)
		{
			pwd_Change();
		}
		else if (m.x > 297 && m.x < 596 && m.y>305 && m.y < 348 && m.uMsg == WM_LBUTTONDOWN)
		{
			admin_Place_Change();
		}
		else if (m.x > 291 && m.x < 596 && m.y>219 && m.y < 267 && m.uMsg == WM_LBUTTONDOWN)
		{
			admin_Place_delete();
		}
	}
}

/*����Ա�޸�����*/
void pwd_Change()
{
	MOUSEMSG m;
	non_edge_rectangle(0, 0, 1000, 600);
	outtextxy(400, 20, _T("����Ա�޸��������"));
	outtextxy(260, 185, _T("ԭ���룺"));
	outtextxy(260, 285, _T("�����룺"));
	outtextxy(690, 375, _T("ȷ��"));
	setcolor(RGB(6, 67, 6));
	line(367, 320, 630, 320);
	line(367, 225, 630, 225);
	int flag_old = 0,flag_new = 0;
	int charx_old = 380, chary_old = 185;//�൱�ھ�����Ĺ��
	int charx_new = 380, chary_new = 285;//������Ĺ��
	char old_pwd[15];
	char new_pwd[15];
	while (1)
	{
		m = GetMouseMsg();
		
		if (m.x > 260 && m.x < 630 && m.y>185 && m.y < 225 && m.uMsg == WM_LBUTTONDOWN && flag_old <= 9)
		{
			InputBox(old_pwd, 10, "�����������", NULL, "�����������", 300, 200);
			//getkb_string(charx_pwd, chary_pwd, pwd, flag_pwd);
			non_edge_rectangle(364, 182, 713, 218);//����ԭʼ��Ϣ
			flag_old = strlen(old_pwd);
			for (int i = 0; i < flag_old; i++)
			{
				outtextxy(charx_old, chary_old, _T("*"));
				charx_old += 20;
			}
			charx_old = 380, chary_old = 185;
		}
		else if (m.x > 260 && m.x < 630 && m.y>285 && m.y < 320 && m.uMsg == WM_LBUTTONDOWN&&flag_new<=9)
		{
			InputBox(new_pwd, 10, "������������", NULL, "������������", 300, 200);
			non_edge_rectangle(368, 279, 816, 311);//����ԭʼ��Ϣ
			flag_new = strlen(new_pwd);
			for (int i = 0; i < flag_new; i++)
			{
				outtextxy(charx_new, chary_new, _T("*"));
				charx_new += 20;
			}
			charx_new = 380, chary_new = 285;
		}
		else if (m.x > 660 && m.x < 860 && m.y>365 && m.y < 425 && m.uMsg == WM_LBUTTONDOWN)
		{
			
			if (strcmp(old_pwd, admin_pwd) == 0 && flag_new < 10)
			{
				strcpy(admin_pwd, new_pwd);
			}
			else
			{
				settextcolor(RGB(248,81,55));
				outtextxy(248, 396, _T("ԭ������������ԣ�"));
				Sleep(2000);
				settextcolor(RGB(6, 67, 6));
			}
			initSystem();
		}
	}
}

/*����Աɾ��У԰����*/
void admin_Place_delete()
{
	MOUSEMSG m;
	non_edge_rectangle(0, 0, 1000, 600);
	IMAGE map;
	loadimage(&map, "res/map.jpg", 430, 600);
	putimage(0, 0, &map);
	settextcolor(RGB(6, 67, 6));
	outtextxy(575, 110, _T("��ѡ��Ҫ��ʱɾ���ĵص�"));
	place_delete = click_place(0);
	outtextxy(575, 180, _T(g->vexs[place_delete - 1].name));
	outtextxy(770, 500, _T("ȷ��"));
	while (1)
	{
		m = GetMouseMsg();
		/*ȷ����*/
		if (m.x > 770 && m.x < 960 && m.y>500 && m.y < 560 && m.uMsg == WM_LBUTTONDOWN)
		{
			initSystem();
		}
	}
	non_edge_rectangle(467, 0, 1000, 600);
}

/*����Ա�޸�У԰����*/
void admin_Place_Change()
{
	MOUSEMSG m;
	int place_change=-1;
	non_edge_rectangle(0, 0, 1000, 600);
	IMAGE map;
	loadimage(&map, "res/map.jpg", 430, 600);
	putimage(0, 0, &map);
	settextcolor(RGB(6, 67, 6));
	outtextxy(575, 110, _T("��ѡ��Ҫ�޸ĵĵص�"));
	place_change = click_place(0);
	outtextxy(575, 180, _T(g->vexs[place_change - 1].name));
	outtextxy(575, 272, _T("�������޸���������"));
	outtextxy(770, 500, _T("ȷ��"));
	while (1)
	{
		m = GetMouseMsg();
		/*ȷ����*/
		if (m.x > 770 && m.x < 960 && m.y>500 && m.y < 560 && m.uMsg == WM_LBUTTONDOWN)
		{
			initSystem();
		}
		else if (m.x > 575 && m.x < 880 && m.y>265 && m.y < 317 && m.uMsg == WM_LBUTTONDOWN)
		{
			InputBox(g->vexs[place_change-1].intro, 200, "�����뾰����Ϣ", NULL, g->vexs[place_change - 1].intro, 500, 300);
		}
	}

	non_edge_rectangle(467, 0, 1000, 600);
}


/*ѡ���ͼ�ϵľ���*/
int click_place(int flag)
{
	int place=-1;
	MOUSEMSG m;
	while (1)
	{
		m = GetMouseMsg();
		if (m.x > 245 && m.x < 275 && m.y>45 && m.y < 85 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=1){return 1;}
		else if (m.x > 360 && m.x < 400 && m.y>15 && m.y < 60 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=2){return 2;}
		else if (m.x > 366 && m.x < 395 && m.y>103 && m.y < 134 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=3){return 3;}
		else if (m.x > 370 && m.x < 400 && m.y>163 && m.y < 202 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=4){return 4;}
		else if (m.x > 325 && m.x < 361 && m.y>142 && m.y < 173 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=5){return 5;}
		else if (m.x > 267 && m.x < 308 && m.y>131 && m.y < 167 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=6){return 6;}
		else if (m.x > 198 && m.x < 243 && m.y>136 && m.y < 176 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=7){return 7;}
		else if (m.x > 100 && m.x < 143 && m.y>226 && m.y < 270 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=8){return 8;}//��ѧ������ҩѧԺ
		else if (m.x > 242 && m.x < 269 && m.y>172 && m.y < 200 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=9){return 9;}
		else if (m.x > 320 && m.x < 354 && m.y>205 && m.y < 227 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=10){return 10;}
		else if (m.x > 340 && m.x < 373 && m.y>226 && m.y < 263 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=11){return 11;}
		else if (m.x > 220 && m.x < 261 && m.y>285 && m.y < 327 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=12){return 12;}
		else if (m.x > 268 && m.x < 295 && m.y>228 && m.y < 247 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=13){return 13;}
		else if (m.x > 223 && m.x < 261 && m.y>221 && m.y < 250 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=14){return 14;}
		else if (m.x > 200 && m.x < 224 && m.y>243 && m.y < 261 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=15){return 15;}
		else if (m.x > 163 && m.x < 198 && m.y>242 && m.y < 262 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=16){return 16;}
		else if (m.x > 144 && m.x < 176 && m.y>202 && m.y < 241 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=17){return 17;}
		else if (m.x > 224 && m.x < 271 && m.y>340 && m.y < 375 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=18){return 18;}
		else if (m.x > 190 && m.x < 228 && m.y>448 && m.y < 477 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=19){return 19;}
		else if (m.x > 144 && m.x < 185 && m.y>464 && m.y < 504 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=20){return 20;}
		else if (m.x > 105 && m.x < 150 && m.y>421 && m.y < 454 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=21){return 21;}
		else if (m.x > 28 && m.x < 78 && m.y>378 && m.y < 433 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=22){return 22;}
		else if (m.x > 12 && m.x < 57 && m.y>454 && m.y < 484 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=23){return 23;}
		else if (m.x > 157 && m.x < 192 && m.y>279 && m.y < 286 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=24){return 24;}
		else if (m.x > 174 && m.x < 205 && m.y>315 && m.y < 339 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=25){return 25;}
		else if (m.x > 108 && m.x < 158 && m.y>356 && m.y < 391 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=26){return 26;}
		else if (m.x > 170 && m.x < 196 && m.y>381 && m.y < 410 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=27){return 27;}
		else if (m.x > 97 && m.x < 141 && m.y>468 && m.y < 485 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=28){return 28;}
		else if (m.x > 59 && m.x < 96 && m.y>436 && m.y < 453 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=29){return 29;}
		else if (m.x > 62 && m.x < 91 && m.y>457 && m.y < 477 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=30){return 30;}
		else if (m.x > 52 && m.x < 86 && m.y>490 && m.y < 501 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=31){return 31;}
		else if (m.x > 6 && m.x < 48 && m.y>500 && m.y < 528 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=32){return 32;}
		else if (m.x > 90 && m.x < 127 && m.y>512 && m.y < 532 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=33){return 33;}
		else if (m.x > 62 && m.x < 96 && m.y>542 && m.y < 562 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=34){return 34;}
		else if (m.x > 272 && m.x < 304 && m.y>328 && m.y < 358 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=35){return 35;}
		else if (m.x > 91 && m.x < 134 && m.y>491 && m.y < 501 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=36){return 36;}
		else if (m.x > 298 && m.x < 330 && m.y>79 && m.y < 108 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=37){return 37;}
		else if (m.x > 395 && m.x < 430 && m.y>73 && m.y < 116 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=38){return 38;}
		else if (m.x > 278 && m.x < 314 && m.y>277 && m.y < 301 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=39){return 39;}
		else if (m.x > 252 && m.x < 279 && m.y>270 && m.y < 284 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=40){return 40;}
		else if (m.x > 247 && m.x < 267 && m.y>158 && m.y < 172 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=41){return 41;}
		else if (m.x > 271 && m.x < 288 && m.y>174 && m.y < 188 && m.uMsg == WM_LBUTTONDOWN&&place_delete!=41){return 41;}

		else if (m.x > 770 && m.x < 960 && m.y>500 && m.y < 560 && m.uMsg == WM_LBUTTONDOWN&& flag==1) { return 999; }
	}
	return place;
}
void outputtext(int x_init, int y, char* text)
{//575 190
	int j = 0;
	int i = 0;
	int len = strlen(text);
	int x = x_init;
	char word[30];
	for (j = 0; j <= len / 24; j++)
	{
		for (i = 0; i < 24&&i+j*24<len; i++)
		{
			word[i] = text[i+j*24];
		}
		word[i] = '\0';
		outtextxy(x, y, word);
		y += 30;
	}
}
void initSystem()
{
	/*��ʼ��ҳ��*/
	MOUSEMSG m;
	IMAGE homepage;

	setcolor(RGB(255, 255, 255));
	fillrectangle(0, 0, 1000, 600);
	loadimage(&homepage, "res/homepageBgd.jpg", 1000, 600);
	putimage(0, 0, &homepage);
	settextcolor(RGB(255, 255, 255));//����������ɫ
	settextstyle(40, 0, _T("����"));//�����ֺš�����
	setbkmode(TRANSPARENT);
	outtextxy(300, 20, _T("���ϴ�ѧУ԰����ϵͳ"));
	setlinestyle(SOLID_FILL, 5);
	settextstyle(30, 0, _T("����"));
	outtextxy(400, 235, _T("����Ա��¼"));//����Ҫ���������
	ellipse(350, 200, 600, 300);
	settextstyle(30, 0, _T("����"));
	outtextxy(410, 385, _T("�ο͵�¼"));
	ellipse(350, 350, 600, 450);
	settextcolor(RGB(6, 67, 6));
	
	while (1)
	{
		/*����Ա��¼*/
		m = GetMouseMsg();
		if (m.x > 350 && m.x < 600 && m.y>200 && m.y < 300 && m.uMsg == WM_LBUTTONDOWN)
		{
			cleardevice();
			non_edge_rectangle(0, 0, 1000, 600);
			admin_UI();
			break;
		}
		/*�ο͵�¼*/
		else if (m.x > 350 && m.x < 600 && m.y>350 && m.y < 450 && m.uMsg == WM_LBUTTONDOWN)
		{
		
			setlinestyle(PS_SOLID, 2);
			for (int i = 0; i <= 300; i++)//����չ���Ľ���
			{
				setlinecolor(RGB(75, 123, 187));
				line(0, i, 999, i);
				line(0, (600 - i), 999, (600 - i));
				Sleep(1);
			}
			cleardevice();
			non_edge_rectangle(0, 0, 1000, 600);
			IMAGE map;
			loadimage(&map, "res/map.jpg", 430, 600);
			putimage(0, 0, &map);
			visitor_UI();
			break;
		}
	}

}
int main()
{
	g = (mgraphtype*)malloc(sizeof(mgraphtype));
	/*��ʼ��ҳ��*/
	initgraph(1000, 600);//�򿪴���
	fillrectangle(0, 0, 1000, 600);
	setcolor(RGB(255, 255, 255));
	
	Create_Map(g);
	Floyd(g);
	initSystem();
	getchar();
	closegraph();//�رմ���
	return 0;
}
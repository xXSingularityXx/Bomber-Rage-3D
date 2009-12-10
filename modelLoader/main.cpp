//#include <iostream>
////#include <GL/glut.h>
//#include <SDL/SDL_opengl.h>
//#include "ImageLoader.h"
//#include "Vector3D.h"
//#include "ObjectManager.h"
//#include "Node.h"
//#include "StaticNode.h"
//#include "Texture.h"
//#include "Terrain.h"
//
//// create all other objects
//ObjectManager* objManager = new ObjectManager();
//
//Node* nodeTest = objManager->createNode("bomber.md2");
//Node* node2 = objManager->createNode("pknight_weapon.MD2");
//
//float animationTime=0.0250f;
//
//GLfloat fAspect;
//
//
///* ANIMATION LIST
//	stand
//	run
//	attack
//	pain1
//	pain2
//	pain3
//	jump
//	flip
//	salute
//	taunt
//	wave
//	point
//	crstnd
//	crwalk
//	crwalk
//	crattk
//	crpain
//	crdeath
//	death1
//	death2
//	death3
//*/
//
//void DefineIluminacao (void)
//{
//	GLfloat luzAmbiente[4]={0.1,0.1,0.1,1.0};
//	GLfloat luzDifusa[4]={0.8,0.6,0.6,1.0};	   // "cor"
//	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
//	GLfloat posicaoLuz[4]={130.0, 100.0, 0.0, 1.0};
//
//	GLint especMaterial = 60;
//
//	// Ativa o uso da luz ambiente
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
//
//	// Define os par�metros da luz de n�mero 0
//	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
//	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
//}
//
//// Fun��o callback de redesenho da janela de visualiza��o
//void Desenha(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	// Limpa a janela de visualiza��o com a cor
//	// de fundo definida previamente
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	// Altera a cor do desenho para preto
//	glColor3f(0.0f, 0.0f, 0.0f);
//
//	DefineIluminacao();
//
//	//desenha os nodos
//	nodeTest->draw();
//	node2->draw();
//
//	// Executa os comandos OpenGL
//	glFlush();
//}
//
//// Fun��o usada para especificar o volume de visualiza��o
//void EspecificaParametrosVisualizacao(void)
//{
//
//	// Especifica sistema de coordenadas de proje��o
//	glMatrixMode(GL_PROJECTION);
//	// Inicializa sistema de coordenadas de proje��o
//	glLoadIdentity();
//
//	// Especifica a proje��o perspectiva(angulo,aspecto,zMin,zMax)
//	gluPerspective(30,fAspect,0.5,500);
//
//	// Especifica sistema de coordenadas do modelo
//	glMatrixMode(GL_MODELVIEW);
//	// Inicializa sistema de coordenadas do modelo
//	glLoadIdentity();
//
//	// Especifica posi��o do observador e do alvo
//	gluLookAt(-30,100,150, 0,0,0, 0,1,0);
//}
//
//// Fun��o callback chamada quando o tamanho da janela � alterado
//void AlteraTamanhoJanela(GLsizei w, GLsizei h)
//{
//	// Para previnir uma divis�o por zero
//	if ( h == 0 ) h = 1;
//
//	// Especifica as dimens�es da viewport
//	glViewport(0, 0, w, h);
//
//	// Calcula a corre��o de aspecto
//	fAspect = (GLfloat)w/(GLfloat)h;
//
//	EspecificaParametrosVisualizacao();
//}
//
//
//
//// Fun��o callback chamada para gerenciar eventos de teclas
//void Teclado (unsigned char key, int x, int y)
//{
//	animationTime=0.0450f;
//	if (key == 27)
//		exit(0);
//	else if (key == 'a')
//	{
//		animationTime=0.0250f;
//		nodeTest->setAnimation("stand");
//		node2->setAnimation("stand");
//
//	}
//	else if (key == 's')
//	{
//		nodeTest->setAnimation("run");
//		node2->setAnimation("run");
//	}
//	else if (key == 'd')
//	{
//		nodeTest->setAnimation("jump");
//		node2->setAnimation("jump");
//	}
//	else if (key == 'f')
//	{
//		nodeTest->setAnimation("attack");
//		node2->setAnimation("attack");
//	}
//}
//
//// Fun��o respons�vel por inicializar par�metros e vari�veis
//void Inicializa (void)
//{
//	// Define a cor de fundo da janela de visualiza��o como branca
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	glLineWidth(2.0);
//
//	glEnable(GL_NORMALIZE);
//	// Habilita a defini��o da cor do material a partir da cor corrente
//	glEnable(GL_COLOR_MATERIAL);
//	//Habilita o uso de ilumina��o
//	glEnable(GL_LIGHTING);
//	// Habilita a luz de n�mero 0
//	glEnable(GL_LIGHT0);
//	// Habilita o depth-buffering
//	glEnable(GL_DEPTH_TEST);
//
//	// Habilita o modelo de coloriza��o de Gouraud
//	glShadeModel(GL_SMOOTH);
//
//	//cria a textura do nodo (kinight)
//	Texture* tex01 = objManager->createTexture("pknight.BMP");
//	nodeTest->setTexture(tex01);
//	delete tex01;
//
//	//cria a textura do nodo (espada)
//	Texture* tex02 = objManager->createTexture("pknight_weapon.BMP");
//	node2->setTexture(tex02);
//	delete tex02;
//
//	delete objManager;
//
//	nodeTest->setAnimation("stand");
//	node2->setAnimation("stand");
//}
//
//
//void update(int _value)
//{
//	// incrementa os frames
//	nodeTest->advanceFrame(animationTime);
//	node2->advanceFrame(animationTime);
//
//	glutPostRedisplay();
//	glutTimerFunc(50, update, 0);
//	glutPostRedisplay();
//}
//
//// Programa Principal
//int main(void)
//{
//	// Define do modo de opera��o da GLUT
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
//
//	// Especifica a posi��o inicial da janela GLUT
//	glutInitWindowPosition(5,5);
//
//	// Especifica o tamanho inicial em pixels da janela GLUT
//	glutInitWindowSize(450,450);
//
//	// Cria a janela passando como argumento o t�tulo da mesma
//	glutCreateWindow("Desenho de um cubo");
//
//	// Registra a fun��o callback de redesenho da janela de visualiza��o
//	glutDisplayFunc(Desenha);
//
//	// Registra a fun��o callback de redimensionamento da janela de visualiza��o
//	glutReshapeFunc(AlteraTamanhoJanela);
//
//	// Registra a fun��o callback para tratamento das teclas ASCII
//	glutKeyboardFunc (Teclado);
//
//	glutTimerFunc(50, update, 0);
//
//	// Chama a fun��o respons�vel por fazer as inicializa��es
//	Inicializa();
//
//	// Inicia o processamento e aguarda intera��es do usu�rio
//	glutMainLoop();
//
//	return 0;
//}

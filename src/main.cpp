#ifndef DBOX
#define DBOX
#include<iostream>
#include<sstream>
#include<cmath>//m3
#include<list>//glob
#include<sys/time.h>//tmr
#include<execinfo.h>//signl
#include<errno.h>//net
#include<sys/socket.h>//net
#include<sys/types.h>//net
#include<netdb.h>//net
#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include"../glfw/glfw.h"
#include<png.h>
#include<thread>
using namespace std;
namespace dbox{
	#define flf()l("···",__FILE__,__LINE__,__FUNCTION__);
	inline ostream&l(const char*s="",const char*file="",int lineno=0,const char*func=""){cerr<<file;if(lineno){cerr<<":"<<lineno;}cerr<<" "<<func<<"  "<<s;return cerr;}
	class clk{
	public:
		int fps=120;
		int dtms=1000/fps;
		float dt=dtms/1000.f;
		long tk=0;
	}clk;
	class metrics{
	public:
		int globs;
		int coldetsph;
		int frames;
		float dtupd;
		float dtrend;
		int p3s;
		int m3s;
		int collisions;
		int mwrfsh;
		int mpmul;
		int mmmul;
		int ngrids;
		float dtcoldetgrd;
		float dtcoldetbrute;
		float dtnet;
		int viewcull;
		int globsrend;
		int rays;
		float rayone;
		int gridsculled;
		int gridsrend;
		float dtgrdput;
	}metrics;
	inline float dt(const float f=1){return f*clk.dt;}
	inline float rnd(const float from,const float tonotincluding){return from+(tonotincluding-from)*rand()/RAND_MAX;}
	inline float rndo(const float tonotincluding){return tonotincluding*rand()/RAND_MAX;}
	inline float rndn(const float s){return rnd(-s,s);}
	const float pi=3.1415926f;
	const float degtoradp=pi/180;
	inline float degtorad(const float deg=1){return deg*degtoradp;}
//	ostringstream sts;

	class tmr{
		struct timeval t0;
	public:
		tmr(){restart();}
		void restart(){gettimeofday(&t0,NULL);}
		float dt(){
			struct timeval tv;
			gettimeofday(&tv,NULL);
			const time_t diff_s=tv.tv_sec-t0.tv_sec;
			const int diff_us=tv.tv_usec-t0.tv_usec;
			t0=tv;
			return (float)diff_s+diff_us/1000000.f;
		}
	}tmr_;
	class signl{
		const int i;
		const char*s;
	public:
		signl(const int i=0,const char*s="signal"):i(i),s(s){
			cerr<<" ••• signl "<<i<<" · "<<s<<endl;
			const int nva=10;
			void*va[nva];
			int n=backtrace(va,nva);
			backtrace_symbols_fd(va,n,1);
		}
		inline int num()const{return i;}
		inline const char* str()const{return s;}
	};

	class shader{
	public:
		GLuint prog=0;
		GLint umxmw=0;
		GLint umxwv=0;
		GLint udopersp=0;
		GLint urendzbuf=0;
		GLint utx=0;
		void init(){
			const GLuint vtxshdr=glCreateShader(GL_VERTEX_SHADER);
	//		const GLchar*vtxshdrsrc[]={"void main(){gl_TexCoord[0]=gl_TextureMatrix[0]*gl_ModelViewMatrix*gl_Vertex;gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;gl_FrontColor=gl_Color;}"};
	//		const GLchar*vtxshdrsrc[]={"varying vec3 vnml;void main(){gl_TexCoord[0]=gl_TextureMatrix[0]*gl_ModelViewMatrix*gl_Vertex;gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;gl_FrontColor=gl_Color;vnml=normalize(gl_NormalMatrix*gl_Normal);}"};
	//		const GLchar*vtxshdrsrc[]={"void main(){gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;gl_FrontColor=gl_Color;gl_TexCoord[1]=gl_Vertex;}"};
	//		const GLchar*vtxshdrsrc[]={"void main(){gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;gl_FrontColor=gl_Color;gl_TexCoord[1]=gl_MultiTexCoord1;}"};
	//		const GLchar*vtxshdrsrc[]={"void main(){gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;gl_FrontColor=gl_Color;gl_TexCoord[1]=gl_MultiTexCoord1;gl_TexCoord[2]=gl_TextureMatrix[2]*gl_ModelViewMatrix*gl_Vertex;}"};
	//		const GLchar*vtxshdrsrc[]={"varying vec3 vnml;void main(){gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;gl_FrontColor=gl_Color;gl_TexCoord[1]=gl_MultiTexCoord1;gl_TexCoord[2]=gl_TextureMatrix[2]*gl_ModelViewMatrix*gl_Vertex;vnml=normalize(gl_NormalMatrix*gl_Normal);}"};
	//		const GLchar*vtxshdrsrc[]={"#version 150 core\nuniform mat4 umxmw;uniform mat4 umxwv;uniform bool udopersp=true;in vec3 in_pos;in vec4 in_rgba;in vec2 in_txc;out vec4 rgba;out vec2 txcoord;void main(){rgba=in_rgba;txcoord=in_txc;vec4 pw=umxmw*vec4(in_pos,1);vec4 pv=umxwv*pw;if(udopersp){pv.w=-pv.z;pv.z*=pv.z;}else{pv.z=-pv.z;}gl_Position=pv;}"};
			const GLchar*vtxshdrsrc[]={"#version 150 core\nuniform mat4 umxmw;uniform mat4 umxwv;uniform bool udopersp=true;in vec3 in_pos;in vec4 in_rgba;in vec2 in_txc;out vec4 rgba;out vec2 txcoord;void main(){rgba=in_rgba;txcoord=in_txc;vec4 pw=umxmw*vec4(in_pos,1);vec4 pv=umxwv*pw;if(udopersp){pv.w=-pv.z;pv.z*=pv.z;}else{pv.z=-pv.z;}gl_Position=pv;}"};
	//		const GLchar*vtxshdrsrc[]={"#version 150 core\nuniform mat4 umxmw;uniform mat4 umxwv;uniform bool udopersp=true;in vec3 in_pos;in vec4 in_rgba;in vec2 in_txc;out vec4 rgba;out vec2 txcoord;void main(){rgba=in_rgba;txcoord=in_txc;vec4 pw=umxmw*vec4(in_pos,1);vec4 pv=umxwv*pw;if(udopersp){pv.w=-pv.z;pv.z*=pv.z;}else{pv.z=-pv.z;}gl_Position=vec4(in_pos,1);}"};
			const GLint vtxshdrsrclen[]={GLint(strlen(vtxshdrsrc[0]))};
			glShaderSource(vtxshdr,1,vtxshdrsrc,vtxshdrsrclen);
			glCompileShader(vtxshdr);
			GLint sts;
			glGetShaderiv(vtxshdr,GL_COMPILE_STATUS,&sts);
			const int n=4*1024;
			char buf[n];
			GLsizei nchs;
			if(!sts){
				glGetShaderInfoLog(vtxshdr,n,&nchs,buf);
				cerr<<"vertex shader did not compile"<<endl<<buf;
				throw signl();
			}
			const GLuint frgshdr=glCreateShader(GL_FRAGMENT_SHADER);
	//		const GLchar*frgshdrsrc[]={"uniform sampler2D ushad;void main(){vec4 shad;shad=texture2DProj(ushad,gl_TexCoord[0]);float la=shad.z<gl_TexCoord[0].z/gl_TexCoord[0].w?.5:.7;gl_FragColor=la*gl_Color;}"};
	//		const GLchar*frgshdrsrc[]={"varying vec3 vnml;uniform sampler2D ushadow0;void main(){vec4 shado;shado=texture2DProj(ushadow0,gl_TexCoord[0]);float la=shado.z<gl_TexCoord[0].z/gl_TexCoord[0].w?0.:.2;float wa=gl_FragCoord.w;vec3 lhta=vec3(1,0,0);float ln=dot(vnml,lhta);gl_FragColor=clamp(la*.5+wa*.5+ln*.2,0.,1.)*gl_Color;}"};
	//		const GLchar*frgshdrsrc[]={"uniform sampler2D utex;void main(){vec4 tex;tex=texture2D(utex,gl_TexCoord[1].st);gl_FragColor=tex;}"};
	//		const GLchar*frgshdrsrc[]={"uniform sampler2D utex;uniform sampler2D ushad;void main(){vec4 tex;tex=texture2D(utex,gl_TexCoord[1].st);vec4 shad;shad=texture2DProj(ushad,gl_TexCoord[2]);float la=shad.z<gl_TexCoord[2].z/gl_TexCoord[2].w?-.2:0.;gl_FragColor=la*vec4(1,1,1,1)+tex+gl_Color;}"};
	//		const GLchar*frgshdrsrc[]={"uniform sampler2D ushad;uniform sampler2D utex;varying vec3 vnml;void main(){vec4 tex;tex=texture2D(utex,gl_TexCoord[1].st);vec4 shad;shad=texture2DProj(ushad,gl_TexCoord[2]);float la=shad.z<gl_TexCoord[2].z/gl_TexCoord[2].w?.5:1.;vec3 lht=vec3(1,0,0);float ln=dot(normalize(vnml),lht);gl_FragColor=la*(tex+ln+gl_Color);}"};
	//		const GLchar*frgshdrsrc[]={"uniform sampler2D ushad;uniform sampler2D utex;varying vec3 vnml;void main(){vec4 tex;tex=texture2D(utex,gl_TexCoord[1].st);vec4 shad;shad=texture2DProj(ushad,gl_TexCoord[2]);float la=shad.z<gl_TexCoord[2].z/gl_TexCoord[2].w?.5:1.;vec3 lht=vec3(1,0,0);float ln=dot(normalize(vnml),lht);float wa=gl_FragCoord.w;gl_FragColor=la*(ln*.2+wa*.5+tex+gl_Color);}"};
			const GLchar*frgshdrsrc[]={"#version 150 core\nuniform sampler2D utx;in vec4 rgba;in vec2 txcoord;out vec4 out_Color;void main(){vec4 txrgba=texture(utx,txcoord);if(txrgba.a==0)discard;out_Color=txrgba+rgba;/*+vec4(txcoord,1,1);*//*rgba+txrgba;*//*+rgba+(1-gl_FragCoord.z);*/}"};
			const GLint frgshdrsrclen[]={GLint(strlen(frgshdrsrc[0]))};
			glShaderSource(frgshdr,1,frgshdrsrc,frgshdrsrclen);
			glCompileShader(frgshdr);
			glGetShaderiv(frgshdr,GL_COMPILE_STATUS,&sts);
			if(!sts){
				glGetShaderInfoLog(frgshdr,n,&nchs,buf);
				cerr<<"frag shader did not compile"<<endl<<buf<<endl;
				throw signl();
			}
			prog=glCreateProgram();
			glAttachShader(prog,vtxshdr);
			glAttachShader(prog,frgshdr);
			glLinkProgram(prog);
			glGetProgramiv(prog,GL_LINK_STATUS,&sts);
			if(!sts){
				glGetProgramInfoLog(prog,n,&nchs,buf);
				cerr<<"program did not link"<<endl<<buf<<endl;
				throw signl();
			}
			if((umxmw=glGetUniformLocation(prog,"umxmw"))==-1)throw signl(0,"umxmw not found");
			if((umxwv=glGetUniformLocation(prog,"umxwv"))==-1)throw signl(0,"umxwv not found");
			if((udopersp=glGetUniformLocation(prog,"udopersp"))==-1)throw signl(0,"udopersp not found");
//			if((urendzbuf=glGetUniformLocation(prog,"urendzbuf"))==-1)throw signl(0,"urendzbuf not found");
			if((utx=glGetUniformLocation(prog,"utx"))==-1)throw signl(0,"utx not found");
			glUseProgram(prog);

			if(glGetError())throw signl(0,"shader::init");
		}
	}shader;
	class net{
//		int keyslen=nplayers*nkeys;
	//	const char*playername="noname";
//		bool sockio=false;
	public:
		const char*host="127.0.0.1";
		const char*port="8085";
		const int nkeys=32;
		int nplayers=1;
		char*keys;//[nplayers][nkeys];
		int player=0;
	protected:
		int sockfd=0;
		struct addrinfo*ai=0;
		char*localkeysbuf;
		void keydn(const int key,const int x=0,const int y=0){
			const int i=keyix(key);
			if(!i)return;
			const int s=keys[player*nkeys+i];
			if(s==1)return;
			keys[player*nkeys+i]=1;
			static int xx,yy;
			xx=x;yy=y;
//			cout<<"keydn("<<(int)key<<",["<<x<<","<<y<<"],"<<key<<")";
	//		sts<<"keydn("<<(int)key<<",["<<x<<","<<y<<"],"<<key<<")";
		}
		void keyup(const int key,const int x=0,const int y=0){
			const int i=keyix(key);
			const int s=keys[player*nkeys+i];
			if(s==0)return;
			if(s==1)return;
			if(s!=2)throw signl(2,"unknownstate");
			keys[player*nkeys+i]=0;
			static int xx,yy;
			xx=x;yy=y;
//			cout<<"keyup("<<(int)key<<",["<<x<<","<<y<<"],"<<key<<")";
	//		sts<<"keyup("<<(int)key<<",["<<x<<","<<y<<"],"<<key<<")";
		}
	public:
		int keyix(const int key){//? char keys[]
			switch(key){
			case 'w':return 1;
			case 'j':return 2;
			case 's':return 3;
			case 'l':return 4;
			case 'a':return 5;
			case 'd':return 6;
			case 'i':return 7;
			case 'k':return 8;
			case 'm':return 9;
			case ',':return 10;
			case 'z':return 11;
			case 'x':return 12;
			case 'c':return 13;
			case ' ':return 14;
			case 'b':return 15;
			case 9:return 16;
			case 27:return 17;
			case 't':return 18;
			case 'g':return 19;
			case 'y':return 20;
			case 'h':return 21;
			case '1':return 22;
			case '2':return 23;
			case '3':return 24;
			case '4':return 25;
			case '5':return 26;
			case '6':return 27;
			case '7':return 28;
			case '8':return 29;
			case '9':return 30;
			case '0':return 31;
			default:return 0;
			}
		}
		void onkeyb(const int c,const bool pressed,const int x,const int y){
			if(pressed)
				keydn(c,x,y);
			else
				keyup(c,x,y);
		}
		void sendkeys(){
//			memcpy(keys+nkeys*player,localkeysbuf,(size_t)nkeys);
			const ssize_t bytes_sent=send(sockfd,localkeysbuf,(size_t)nkeys,0);
			//flf();l("sent keys for player ")<<player<<"  bytessent("<<bytes_sent<<endl;
			if(bytes_sent==-1){flf();l(strerror(errno))<<endl;throw signl(1,"sendkeys");}
		}
		void reckeys(){
			const ssize_t reclen=recv(sockfd,keys,(size_t)(nplayers*nkeys),0);
			if(reclen==0){flf();l("closed")<<endl;exit(1);}
			if(reclen==-1){flf();l(strerror(errno))<<endl;exit(2);}
			if(reclen!=nplayers*nkeys)throw signl(3,"uncompleterec");//?
	//		print();
		}
		void init0(){
			localkeysbuf=new char[nkeys];
			memset(localkeysbuf,0,(size_t)nkeys);
			keys=new char[nplayers*nkeys];
			memset(keys,0,(size_t)(nplayers*nkeys));
		}//? leak
		void init(){
			init0();
			flf();l()<<"connect "<<host<<":"<<port<<endl;
			struct addrinfo hints;
			memset(&hints,0,sizeof hints);
			hints.ai_family=AF_UNSPEC;
			hints.ai_socktype=SOCK_STREAM;
			if(getaddrinfo(host,port,&hints,&ai)){flf();l(strerror(errno))<<endl;throw signl();}
			sockfd=socket(ai->ai_family,ai->ai_socktype,ai->ai_protocol);
			if(sockfd==-1){flf();l(strerror(errno))<<endl;throw signl();}
		//	flf();l()<<"socket "<<sockfd<<"  errno("<<errno<<")"<<endl;
			if(connect(sockfd,ai->ai_addr,ai->ai_addrlen)){flf();l(strerror(errno))<<endl;throw signl();}
			flf();l("connected")<<endl;
			const char c[]="0123456789abcdef";
			srand((unsigned int)time(NULL));
			for(int i=0;i<nkeys;i++){
				localkeysbuf[i]=c[(int)rand()%16];
			}
			srand(0);
			flf();cout<<"  keys:"<<sizeof keys<<"\n";
			flf();cout<<"  player id: "<<localkeysbuf<<"\n";
			sendkeys();
			flf();l("waiting for other players.")<<endl;
			reckeys();
			flf();l("all players connected.")<<endl;
			player=-1;
			for(int i=0;i<nplayers;i++){
				bool found=true;
				for(int j=0;j<nkeys;j++){
					if(localkeysbuf[j]!=keys[i*nkeys+j]){
						found=false;
						break;
					}
				}
				if(!found)continue;
				player=i;
				break;
			}
			if(player==-1)throw signl();
			flf();l("u r player #")<<player<<endl;
			memset(localkeysbuf,0,(size_t)(nkeys));
			memset(keys,0,(size_t)(nplayers*nkeys));

		}
		//	void stop(){
		//		if(sockfd&&close(sockfd)){flf();l(strerror(errno))<<endl;}
		//		if(ai)freeaddrinfo(ai);
		//	}
	}net;

	class pt{
		float x,y,z;
	public:
		inline pt():x(0),y(0),z(0){metrics.p3s++;}
		inline pt(const pt&p){metrics.p3s++;x=p.x;y=p.y;z=p.z;}
		inline pt(const float x,const float y,const float z):x(x),y(y),z(z){metrics.p3s++;}
		inline pt(const pt&from,const pt&to):x(to.x-from.x),y(to.y-from.y),z(to.z-from.z){metrics.p3s++;}
		inline pt(const pt&from,const pt&to,const float len):x(to.x-from.x),y(to.y-from.y),z(to.z-from.z){metrics.p3s++;norm(len);}
		inline ~pt(){metrics.p3s--;}
		inline float getx()const{return x;}
		inline pt&setx(const float f){x=f;return*this;}
		inline float gety()const{return y;}
		inline pt&sety(const float f){y=f;return*this;}
		inline float getz()const{return z;}
		inline pt&transl(const float dx,const float dy,const float dz){x+=dx;y+=dy;z+=dz;return*this;}
		inline pt&transl(const pt&d){x+=d.x;y+=d.y;z+=d.z;return*this;}
		inline pt&transl(const pt&d,const float dt){x+=d.x*dt;y+=d.y*dt;z+=d.z*dt;return*this;}
		inline float magn()const{return sqrt(x*x+y*y+z*z);}
		inline float magn2()const{return x*x+y*y+z*z;}//? dot(*this);
		inline pt&set(const pt&p){x=p.x;y=p.y;z=p.z;return*this;}
		inline pt&set(const float x,const float y,const float z){this->x=x;this->y=y;this->z=z;return*this;}
		inline pt&neg(){x=-x;y=-y;z=-z;return*this;}
		inline pt&negy(){y=-y;return*this;}
		inline pt&scale(const float s){x*=s;y*=s;z*=s;return*this;}
		inline pt&scale(const float sx,const float sy,const float sz){x*=sx;y*=sy;z*=sz;return*this;}
		inline bool operator==(const pt&p)const{return x==p.x&&y==p.y&&z==p.z;}
		inline float dot(const pt&p)const{return x*p.x+y*p.y+z*p.z;}
		inline pt&vecprod(const pt&v1,const pt&v2){x=v1.y*v2.z-v1.z*v2.y;y=v1.z*v2.x-v1.x*v2.z;z=v1.x*v2.y-v1.y*v2.x;return*this;}
		inline pt&pow2(){x*=x;y*=y;z*=z;return*this;}
		inline float sum()const{return x+y+z;}
		inline pt&mult(const pt&p){x*=p.x;y*=p.y;z*=p.z;return*this;}
		inline pt&clr(){x=y=z=0;return*this;}
		pt&norm(const float length=1){
			float q=sqrt(x*x+y*y+z*z);
			if(q==0){
				x=y=z=0;
				return*this;
			}
			const float t=length/q;
			x=t*x;y=t*y;z=t*z;
			return*this;
		}
		friend ostream&operator<<(ostream&,const pt&);
		friend istream&operator>>(istream&,pt&);
	};
	inline ostream&operator<<(ostream&os,const pt&p){os<<p.x<<" "<<p.y<<" "<<p.z;return os;}
	inline istream&operator>>(istream&is,pt&p){is>>p.x;is.ignore();is>>p.y;is.ignore();is>>p.z;return is;}

	class mtx{
	public:
		void togl(GLfloat m[])const{
			 m[0]=xx; m[1]=xy; m[2]=xz; m[3]=xo;
			 m[4]=yx; m[5]=yy; m[6]=yz; m[7]=yo;
			 m[8]=zx; m[9]=zy;m[10]=zz;m[11]=zo;
			m[12]=ox;m[13]=oy;m[14]=oz;m[15]=oo;
		}
		inline mtx(){metrics.m3s++;ident();}
		inline mtx(const pt&p,const pt&a){metrics.m3s++;mw(p,a);}
	//	inline m3(const GLfloat*m){metrics.m3s++;set(m);}
		inline ~mtx(){metrics.m3s--;}
		inline pt xaxis()const{return pt(xx,xy,xz);}
		inline pt yaxis()const{return pt(yx,yy,yz);}
		inline pt zaxis()const{return pt(zx,zy,zz);}
//		inline m3&xplane(GLfloat*v){v[0]=xx;v[1]=yx;v[2]=zx;v[3]=ox;return*this;}
//		inline m3&yplane(GLfloat*v){v[0]=xy;v[1]=yy;v[2]=zy;v[3]=oy;return*this;}
//		inline m3&zplane(GLfloat*v){v[0]=xz;v[1]=yz;v[2]=zz;v[3]=oz;return*this;}
//		inline m3&wplane(GLfloat*v){v[0]=xo;v[1]=yo;v[2]=zo;v[3]=oo;return*this;}
		mtx&ident(){xx=1;xy=0;xz=0;xo=0; yx=0;yy=1;yz=0;yo=0; zx=0;zy=0;zz=1;zo=0; ox=oy=oz=0; oo=1;return*this;}
		mtx&wv(const pt&p,const pt&a){
			ident();
			ox=-p.getx();
			oy=-p.gety();
			oz=-p.getz();
			rotz(degtorad(-a.getz()));
			rotx(degtorad(-a.getx()));
			roty(degtorad(-a.gety()));
			return*this;
		}
		mtx&mw(const pt&p,const pt&a){//? Mszxyt
			ident();
			roty(degtorad(a.gety()));
			rotx(degtorad(a.getx()));
			rotz(degtorad(a.getz()));
			ox=p.getx();
			oy=p.gety();
			oz=p.getz();
			return*this;
		}
		const mtx&trnsf(const pt&src,pt&dst)const{
			metrics.mpmul++;
			const float x=src.getx();
			const float y=src.gety();
			const float z=src.getz();
			const float nx=x*xx+y*yx+z*zx+ox;
			const float ny=x*xy+y*yy+z*zy+oy;
			const float nz=x*xz+y*yz+z*zz+oz;
			dst.set(nx,ny,nz);
			return*this;
		}
	//	m3&set(const GLfloat m[16]){
	//		xx=m[ 0];yx=m[ 4];zx=m[ 8];ox=m[12];
	//		xy=m[ 1];yy=m[ 5];zy=m[ 9];oy=m[13];
	//		xz=m[ 2];yz=m[ 6];zz=m[10];oz=m[14];
	//		xo=m[ 3];yo=m[ 7];zo=m[11];oo=m[15];
	//		return*this;
	//	}
		mtx&mul(const mtx&m){
			metrics.mmmul++;
			const float nxx=m.xx*xx+m.yx*xy+m.zx*xz+m.ox*xo;
			const float nyx=m.xx*yx+m.yx*yy+m.zx*yz+m.ox*yo;
			const float nzx=m.xx*zx+m.yx*zy+m.zx*zz+m.ox*zo;
			const float nox=m.xx*ox+m.yx*oy+m.zx*oz+m.ox*oo;

			const float nxy=m.xy*xx+m.yy*xy+m.zy*xz+m.oy*xo;
			const float nyy=m.xy*yx+m.yy*yy+m.zy*yz+m.oy*yo;
			const float nzy=m.xy*zx+m.yy*zy+m.zy*zz+m.oy*zo;
			const float noy=m.xy*ox+m.yy*oy+m.zy*oz+m.oy*oo;

			const float nxz=m.xz*xx+m.yz*xy+m.zz*xz+m.oz*xo;
			const float nyz=m.xz*yx+m.yz*yy+m.zz*yz+m.oz*yo;
			const float nzz=m.xz*zx+m.yz*zy+m.zz*zz+m.oz*zo;
			const float noz=m.xz*ox+m.yz*oy+m.zz*oz+m.oz*oo;

			const float nxo=m.xo*xx+m.yo*xy+m.zo*xz+m.oo*xo;
			const float nyo=m.xo*yx+m.yo*yy+m.zo*yz+m.oo*yo;
			const float nzo=m.xo*zx+m.yo*zy+m.zo*zz+m.oo*zo;
			const float noo=m.xo*ox+m.yo*oy+m.zo*oz+m.oo*oo;

			xx=nxx;yx=nyx;zx=nzx;ox=nox;
			xy=nxy;yy=nyy;zy=nzy;oy=noy;
			xz=nxz;yz=nyz;zz=nzz;oz=noz;
			xo=nxo;yo=nyo;zo=nzo;oo=noo;

			return*this;
		}
		mtx&mul2(const mtx&m){
			metrics.mmmul++;
			float nxx=xx*m.xx+yx*m.xy+zx*m.xz+ox*m.xo;
			float nyx=xx*m.yx+yx*m.yy+zx*m.yz+ox*m.yo;
			float nzx=xx*m.zx+yx*m.zy+zx*m.zz+ox*m.zo;
			float nox=xx*m.ox+yx*m.oy+zx*m.oz+ox*m.oo;

			float nxy=xy*m.xx+yy*m.xy+zy*m.xz+oy*m.xo;
			float nyy=xy*m.yx+yy*m.yy+zy*m.yz+oy*m.yo;
			float nzy=xy*m.zx+yy*m.zy+zy*m.zz+oy*m.zo;
			float noy=xy*m.ox+yy*m.oy+zy*m.oz+oy*m.oo;

			float nxz=xz*m.xx+yz*m.xy+zz*m.xz+oz*m.xo;
			float nyz=xz*m.yx+yz*m.yy+zz*m.yz+oz*m.yo;
			float nzz=xz*m.zx+yz*m.zy+zz*m.zz+oz*m.zo;
			float noz=xz*m.ox+yz*m.oy+zz*m.oz+oz*m.oo;

			float nxo=xo*m.xx+yo*m.xy+zo*m.xz+oo*m.xo;
			float nyo=xo*m.yx+yo*m.yy+zo*m.yz+oo*m.yo;
			float nzo=xo*m.zx+yo*m.zy+zo*m.zz+oo*m.zo;
			float noo=xo*m.ox+yo*m.oy+zo*m.oz+oo*m.oo;

			xx=nxx;yx=nyx;zx=nzx;ox=nox;
			xy=nxy;yy=nyy;zy=nzy;oy=noy;
			xz=nxz;yz=nyz;zz=nzz;oz=noz;
			xo=nxo;yo=nyo;zo=nzo;oo=noo;

			return*this;
		}
		mtx&setsclagltrans(const pt&s,const pt&a,const pt&p){
			const float sinz=sinf(degtorad(a.getz()));
			const float cosz=cosf(degtorad(a.getz()));
			const float sx=s.getx();
			const float sy=s.gety();
			const float sz=s.getz();
			xx=cosz*sx; yx=-sinz*sx; zx=  0; ox=p.getx();
			xy=sinz*sy; yy= cosz*sy; zy=  0; oy=p.gety();
			xz=      0; yz=       0; zz= sz; oz=p.getz();
			xo=      0; yo=       0; zo=  0; oo=1;
			return*this;
		}
		friend ostream&operator<<(ostream&,const mtx&);
		friend istream&operator>>(istream&,mtx&);
	private:
		float xx,yx,zx,ox;
		float xy,yy,zy,oy;
		float xz,yz,zz,oz;
		float xo,yo,zo,oo;
		mtx&rotx(const float a){
			const float c=cos(a),s=sin(a);
			const float nyx=yx*c+zx*s,nyy=yy*c+zy*s,nyz=yz*c+zz*s,nyo=yo*c+zo*s;
			const float nzx=zx*c-yx*s,nzy=zy*c-yy*s,nzz=zz*c-yz*s,nzo=zo*c-yo*s;
			yx=nyx;yy=nyy;yz=nyz;yo=nyo;
			zx=nzx;zy=nzy;zz=nzz;zo=nzo;
			return*this;
		}
		mtx&roty(const float a){
			const float c=cos(a),s=sin(a);
			const float nxx=xx*c-zx*s,nxy=xy*c-zy*s,nxz=xz*c-zz*s,nxo=xo*c-zo*s;
			const float nzx=zx*c+xx*s,nzy=zy*c+xy*s,nzz=zz*c+xz*s,nzo=zo*c+xo*s;
			xx=nxx;xy=nxy;xz=nxz;xo=nxo;
			zx=nzx;zy=nzy;zz=nzz;zo=nzo;
			return*this;
		}
		mtx&rotz(const float a){
			const float c=cos(a),s=sin(a);
			const float nxx=xx*c+yx*s,nxy=xy*c+yy*s,nxz=xz*c+yz*s,nxo=xo*c+yo*s;
			const float nyx=yx*c-xx*s,nyy=yy*c-xy*s,nyz=yz*c-xz*s,nyo=yo*c-xo*s;
			xx=nxx;xy=nxy;xz=nxz;xo=nxo;
			yx=nyx;yy=nyy;yz=nyz;yo=nyo;
			return*this;
		}
	};
	ostream&operator<<(ostream&os,const mtx&m){
		cout<<"["<<pt(m.xx,m.yx,m.zx)<<" "<<m.ox<<"] [";
		cout<<pt(m.xy,m.yy,m.zy)<<" "<<m.oy<<"] [";
		cout<<pt(m.xz,m.yz,m.zz)<<" "<<m.oz<<"] [";
		cout<<pt(m.xo,m.yo,m.zo)<<" "<<m.oo<<"]";
		return os;
	}
	istream&operator>>(istream&is,mtx&m){
		pt p;
		is.ignore();
		is>>p;m.xx=p.getx();m.yx=p.gety();m.zx=p.getz();is.ignore();is>>m.ox;is.ignore(3);
		is>>p;m.xy=p.getx();m.yy=p.gety();m.zy=p.getz();is.ignore();is>>m.oy;is.ignore(3);
		is>>p;m.xz=p.getx();m.yz=p.gety();m.zz=p.getz();is.ignore();is>>m.oz;is.ignore(3);
		is>>p;m.xo=p.getx();m.yo=p.gety();m.zo=p.getz();is.ignore();is>>m.oo;is.ignore();
		return is;
	}

	//#include<png.h>
	void loadfilepng(const char*path,unsigned int&wi,unsigned int&hi,unsigned char*&rgba){
		FILE *fp;
		if((fp=fopen(path,"rb"))==NULL)signl(1,path);//? leak png,pnginfo
		png_structp png=png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
		png_infop pnginfo=png_create_info_struct(png);
		png_init_io(png,fp);
		//PNG_TRANSFORM_STRIP_16 PNG_TRANSFORM_PACKING PNG_TRANSFORM_EXPAND
		png_read_png(png,pnginfo,0,NULL);
		int colortype,interlacetype,bitdepth;
		png_get_IHDR(png,pnginfo,&wi,&hi,&bitdepth,&colortype,&interlacetype,NULL,NULL);
		//? assert valid rgba
		const size_t nrowbytes=png_get_rowbytes(png,pnginfo);
		rgba=(unsigned char*)malloc(nrowbytes*(size_t)hi);
		const png_bytepp pointers=png_get_rows(png,pnginfo);
		for(unsigned int i=0;i<hi;i++)
			memcpy(rgba+nrowbytes*i,pointers[i],nrowbytes);
		png_destroy_read_struct(&png,&pnginfo,NULL);
		fclose(fp);
	}

	class texture{
		GLuint id,wi,hi;
		const char*path;
	public:
		texture():id(0),wi(0),hi(0),path(0){}
		inline GLuint getid()const{return id;}
//		texture(const texture&t):id(t.id),wi(t.wi),hi(t.wi){}
//		texture(const GLuint id,const GLuint wi,const GLuint hi):
//			id(id),wi(wi),hi(hi),path(0)
//		{}
		inline void glbind()const{glBindTexture(GL_TEXTURE_2D,id);}
		void glload(const GLubyte*rgba,const GLuint wi,const GLuint hi){
		    glbind();
//		    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,(GLsizei)wi,(GLsizei)hi,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba);
		    glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			glBindTexture(GL_TEXTURE_2D,0);
		}
		void glloadfile(const char*pth="sprite0.png"){
			path=pth;
			GLubyte*rgba;
		    loadfilepng(path,wi,hi,rgba);
			glGenTextures(1,&id);
		    cout<<" "<<id<<" "<<path<<" "<<wi<<"x"<<hi<<" "<<wi*hi*4/1024<<" KB"<<endl;
		    glload(rgba,wi,hi);
		    delete rgba;
		}
		void genrandom(const int wi=64,const int hi=64){
			int n=4*wi*hi;
			GLubyte*rgba=new GLubyte[n];//? leak
			GLubyte*p=rgba;
			while(n){
				p[0]=(GLubyte)rnd(0,256);
				p[1]=(GLubyte)rnd(0,256);
				p[2]=(GLubyte)rnd(0,256);
				p[3]=255;
				p+=4;
				n-=4;
			}
			glload(rgba,(GLuint)wi,(GLuint)hi);
		}
	};

	class plane:public pt{
		const pt n;
	public:
		plane(const pt&p,const pt&n):
			pt(p),
			n(n)
		{}
		float dist(const pt&p)const{
			const pt v(*this,p);
			return n.dot(v);
		}
	};

	class bvol{
		list<plane*>pnls;
	public:
		~bvol(){
			for(auto pn:pnls)
				delete pn;
//			pnls.clear();
		}
		void addplane(/*own*/plane*pn){
			pnls.push_back(pn);
		}
		int cull(const pt&p,const float r)const{
			for(auto pn:pnls){
				const plane&pp=*pn;
				const float t=pp.dist(p);//? dist2
				if(t>0){// infront
					if(t>r){// infronter than radius, culled
						return 1;
					}
				}
			}
			return 0;
		}
	};

	//#include <list>
	class vbo{
		GLuint glva;//vertex array
		GLuint glvib;//indices array
		GLuint glvb;
		GLsizei nind;
		texture txp;
		bvol bv;
	public:
		virtual void init0(){}
		vbo():glva(0),glvib(0),glvb(0),nind(0){}
		virtual~vbo(){}
		virtual inline const char*name()const{return "vbo";}
		virtual inline int nvertices()const{return 4;}
		virtual inline int elemtype()const{return 0;}
		virtual void vertices(float fa[])const{
			const float w=1;
			int c=0;
			//0
			fa[c++]=-w;fa[c++]=w;fa[c++]=0;//xyz
			fa[c++]= 1;fa[c++]=0;fa[c++]=0;fa[c++]=1;//rgba
			fa[c++]= 0;fa[c++]=0;//st
	//		//1
			fa[c++]=-w;fa[c++]=-w;fa[c++]=0;//xyz
			fa[c++]= 0;fa[c++]=1;fa[c++]=0;fa[c++]=1;//rgba
			fa[c++]= 0;fa[c++]=1;//st
	//		//2
			fa[c++]= w;fa[c++]=-w;fa[c++]=0;//xyz
			fa[c++]= 0;fa[c++]=0;fa[c++]=1;fa[c++]=1;//rgba
			fa[c++]= 1;fa[c++]=1;//st
	//		//3
			fa[c++]= w;fa[c++]= w;fa[c++]=0;//xyz
			fa[c++]= 1;fa[c++]=1;fa[c++]=1;fa[c++]=1;//rgba
			fa[c++]= 1;fa[c++]=0;//st
		}
		virtual GLsizei nindices(){return 6;}
		virtual void indices(char ba[])const{
			int c=0;
			ba[c++]=0;ba[c++]=1;ba[c++]=2;
			ba[c++]=2;ba[c++]=3;ba[c++]=0;
		}
//		virtual inline const char*teximgpath()const{return "sprite0.png";}
		virtual inline const char*teximgpath()const{return 0;}
		virtual inline const bvol getbvol(){return bvol();}
		void glload(){
			cout<<name()<<endl;
			bv=getbvol();
			init0();
			if(glGetError()!=GL_NO_ERROR)throw signl(0,"opengl in error state");
			const int stride=9;//xyz,rgba,st
			const int sizeofnum=sizeof(float);//sizeof(float)
			const int stridebytes=stride*sizeofnum;

			const int nv=nvertices();
			cout<<"  "<<nv<<" vertices, "<<stridebytes<<" B/vertex, "<<(nv*stride*(int)sizeof(float))<<" B"<<endl;
			float*vb=new float[nv*stride];
			vertices(vb);
			glGenVertexArrays(1,&glva);
			glBindVertexArray(glva);
			glGenBuffers(1,&glvb);
			glBindBuffer(GL_ARRAY_BUFFER,glvb);
			if(glGetError()!=GL_NO_ERROR)throw signl(0,"opengl in error state");
			glBufferData(GL_ARRAY_BUFFER,nv*stridebytes,vb,GL_STATIC_DRAW);

			const GLenum err=glGetError();
			if(err==GL_INVALID_ENUM)cout<<"opengl error: GL_INVALID_ENUM"<<endl;
			else if(err==GL_INVALID_VALUE)cout<<"opengl error: GL_INVALID_VALUE"<<endl;
			else if(err==GL_INVALID_OPERATION)cout<<"opengl error: GL_INVALID_OPERATION"<<endl;
			else if(err==GL_OUT_OF_MEMORY)cout<<"opengl error: GL_OUT_OF_MEMORY"<<endl;

			//		glBufferData(GL_ARRAY_BUFFER,nv*stridebytes,vb,GL_STATIC_DRAW);
			if(err!=GL_NO_ERROR)throw signl(0,"opengl in error state");
			glVertexAttribPointer(0,3,GL_FLOAT,false,stridebytes,0);// positions
			glVertexAttribPointer(1,4,GL_FLOAT,false,stridebytes,(const GLvoid*)(3*sizeofnum));// colors, 16 bytes offset
			glVertexAttribPointer(2,2,GL_FLOAT,false,stridebytes,(const GLvoid*)(7*sizeofnum));// texture, 32 bytes offset
			if(glGetError()!=GL_NO_ERROR)throw signl(0,"opengl in error state");
			glBindBuffer(GL_ARRAY_BUFFER,0);
			glBindVertexArray(0);
			if(glGetError()!=GL_NO_ERROR)throw signl(0,"opengl in error state");
			delete vb;

			nind=nindices();
			cout<<"  "<<nind<<" indices, 1 B/index"<<endl;
			char*ib=new char[nind];
			indices(ib);
			glGenBuffers(1,&glvib);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,glvib);
			const GLsizeiptr n=(GLsizeiptr)(nind*(int)sizeof(char));
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,n,ib,GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
			delete ib;

			const char*path=teximgpath();
			if(path){
				cout<<"  texture";
				txp.glloadfile(path);
			}
		}
		void gldraw()const{
			glBindVertexArray(glva);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			if(txp.getid()){//? id==0 notexture
				glEnableVertexAttribArray(2);
				glUniform1i(shader.utx,3);
				glActiveTexture(GL_TEXTURE3);
			}
			txp.glbind();

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,glvib);//? only for elem ops
			switch(elemtype()){
			case 0:
				glDrawElements(GL_TRIANGLES,nind,GL_UNSIGNED_BYTE,0);
				break;
			case 1:
				glDrawElements(GL_TRIANGLE_FAN,nind,GL_UNSIGNED_BYTE,0);
				break;
			case 2:
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glDrawElements(GL_TRIANGLES,nind,GL_UNSIGNED_BYTE,0);
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				break;
			case 3:
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glDisableVertexAttribArray(2);
	//			glDisableVertexAttribArray(1);
				glDrawElements(GL_TRIANGLE_FAN,nind,GL_UNSIGNED_BYTE,0);
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				break;
			case 4:
				glDrawElements(GL_TRIANGLE_FAN,nind,GL_UNSIGNED_BYTE,0);
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glPolygonOffset(-.1f,.1f);
				glDisableVertexAttribArray(2);
				glDisableVertexAttribArray(1);
				glDrawElements(GL_TRIANGLE_FAN,nind,GL_UNSIGNED_BYTE,0);
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				break;
			case 5:
				glDrawArrays(GL_POINTS,0,nvertices());
				break;
			case 6:
				glDrawArrays(GL_LINES,0,nvertices());
				break;
			case 7:
				glDrawArrays(GL_LINE_LOOP,0,nvertices());
				break;
			default:throw signl(0,"unknown elemtype");
			}
		}
	}vbo_;
namespace vbos{
	class axis:public vbo{
	public:
		inline const char*name()const{return "axis";}
		inline int elemtype()const{return 6;}
		inline int nvertices()const{return 6;}
		inline GLsizei nindices(){return 0;}
//		inline GLsizei nindices()const{return 0;}
		void vertices(float fa[])const{
			int j=0;
			fa[j++]=0;fa[j++]=0;fa[j++]=0;//xyz
			fa[j++]=0;fa[j++]=0;fa[j++]=0;fa[j++]=1;//rgba
			fa[j++]=0;fa[j++]=0;//st

			fa[j++]=1;fa[j++]=0;fa[j++]=0;//xyz
			fa[j++]=1;fa[j++]=0;fa[j++]=0;fa[j++]=1;//rgba
			fa[j++]=1;fa[j++]=0;//st

			fa[j++]=0;fa[j++]=0;fa[j++]=0;//xyz
			fa[j++]=0;fa[j++]=0;fa[j++]=0;fa[j++]=1;//rgba
			fa[j++]=0;fa[j++]=0;//st

			fa[j++]=0;fa[j++]=1;fa[j++]=0;//xyz
			fa[j++]=0;fa[j++]=1;fa[j++]=0;fa[j++]=1;//rgba
			fa[j++]=0;fa[j++]=1;//st

			fa[j++]=0;fa[j++]=0;fa[j++]=0;//xyz
			fa[j++]=0;fa[j++]=0;fa[j++]=0;fa[j++]=1;//rgba
			fa[j++]=0;fa[j++]=0;//st

			fa[j++]=0;fa[j++]=0;fa[j++]=1;//xyz
			fa[j++]=0;fa[j++]=0;fa[j++]=1;fa[j++]=1;//rgba
			fa[j++]=0;fa[j++]=0;//st
		}
	}axis;

	class circlexy:public vbo{
	public:
		inline const char*name()const{return "circlexy";}
		inline int elemtype()const{return 7;}
		inline int nvertices()const{return 32;}
		inline GLsizei nindices(){return 0;}
//		inline GLsizei nindices()const{return 0;}
		void vertices(float fa[])const{
			const float da=2*pi/nvertices();
			int j=0;
			float a=0;
			for(int i=0;i<nvertices();i++,a+=da){
				const float c=cosf(a);
				const float s=sinf(a);
				fa[j++]=c;fa[j++]=s;fa[j++]=0;//xyz
				fa[j++]=1;fa[j++]=1;fa[j++]=1;fa[j++]=1;//rgba
				fa[j++]=c;fa[j++]=s;//st
			}
		}
	}circlexy;

	class squarexy:public vbo{
	public:
		inline const char*name()const{return "squarexy";}
		inline int elemtype()const{return 7;}
	}squarexy;

	class spritexy:public vbo{
	public:
		inline const char*name()const{return "spritexy";}
		inline const char*teximgpath()const{return "sprite0.png";}
		virtual void vertices(float fa[])const{
			const float w=1;
			int c=0;
			//0
			fa[c++]=-w;fa[c++]=w;fa[c++]=0;//xyz
			fa[c++]= 0;fa[c++]=0;fa[c++]=0;fa[c++]=0;//rgba
			fa[c++]= 0;fa[c++]=0;//st
	//		//1
			fa[c++]=-w;fa[c++]=-w;fa[c++]=0;//xyz
			fa[c++]= 0;fa[c++]=0;fa[c++]=0;fa[c++]=0;//rgba
			fa[c++]= 0;fa[c++]=1;//st
	//		//2
			fa[c++]= w;fa[c++]=-w;fa[c++]=0;//xyz
			fa[c++]= 0;fa[c++]=0;fa[c++]=0;fa[c++]=0;//rgba
			fa[c++]= 1;fa[c++]=1;//st
	//		//3
			fa[c++]= w;fa[c++]= w;fa[c++]=0;//xyz
			fa[c++]= 0;fa[c++]=0;fa[c++]=0;fa[c++]=0;//rgba
			fa[c++]= 1;fa[c++]=0;//st
		}
	}spritexy_;
}

	class glob:public pt{
		const int id;
		glob&g;
		pt a;
	protected:
		list<glob*>chs;
		list<glob*>chsrm;
		list<glob*>chsadd;
	public:
		int bits;
	private:
		long ptmxupdtk;
		long mxmwtk;
		mtx mxmw;
		pt mxmwpos;
		pt mxmwagl;
		pt mxmwscl;
		bool rmed;
		float r;
		pt dd;
		float bf;
		float m=1;
		long tk;
		long culldrawtk;
	protected:
		pt d;
		pt da;
		pt f;
		pt fi;
		pt pp;
		bool ppsaved;
		inline const list<glob*>getchs()const{return chs;}
		vbo*vb;
		pt scl;
		pt np;
		pt nd;
	public:
		static bool drawboundingspheres;
		static int drawboundingspheresdetail;
		static bool drawaxis;

		inline glob&pos(const pt&coord,const pt&agl){np.set(coord);a.set(agl);return*this;}
		inline glob&setvbo(vbo&v){vb=&v;return*this;}
		inline const mtx&getmxmw(){refreshmxmw();return mxmw;}
		inline glob&dpos(const pt&dpdt,const pt&dadt){nd.set(dpdt);da.set(dadt);return*this;}
		inline const pt&getscl(){return scl;}
		inline glob&setscl(const pt&s){scl.set(s);return*this;}

		glob(glob&g,const pt&p=pt(),const pt&a=pt(),const float r=1,const float density_gcm3=1,const float bounciness=.5f,vbo&vb=*(vbo*)0):
			pt(p),id(metrics.globs++),g(g),a(a),bits(1),ptmxupdtk(-1),mxmwtk(0),rmed(false),
			 r(r),bf(bounciness),m(density_gcm3*4/3*pi*r*r*r),
			 tk(0),culldrawtk(0),d(pt()),da(pt()),f(pt()),fi(pt()),pp(p),ppsaved(false),
			 vb(&vb),scl(r,r,r),
			 np(p),nd(d)
		{
			if(&g==0)return;
			g.chsadd.push_back(this);
		}
		virtual~glob(){
			metrics.globs--;
			for(auto g:chs)
				delete g;
			chs.clear();
		}
		void rm(){
			if(rmed){
	//			flf();l("rmingarmedobj")<<endl;
				return;
			}
			rmed=true;g.chsrm.push_back(this);
		}
		void coldet(glob&o){
			metrics.coldetsph++;
			const pt wpthis=g.posinwcs(*this);
			const pt wpo=o.g.posinwcs(o);
			const pt v(wpthis,wpo);
			const float rr=radius()+o.radius();
			const float rr2=rr*rr;
//			const float d=v.magn();//? magn2
			const float d2=v.magn2();
			if(d2>=rr2){
				if(o.iscoldetrec()){
					for(auto gg:o.chs)
						coldet(*gg);
					return;
				}
				return;
			}
			if(issolid()&&o.issolid()){
				oncol(o);
				o.oncol(*this);
				return;
			}
			if(issolid()&&!o.issolid()){
				for(auto gg:o.chs)
					coldet(*gg);
				return;
			}
			if(!issolid()&&o.issolid()){
				for(auto gg:chs)
					o.coldet(*gg);
				return;
			}
		}
		void culldraw(const bvol&bv){
			if(culldrawtk==clk.tk){
//				flf();l("double rend");
				return;
			}
			culldrawtk=clk.tk;

			const float r=radius();
			const int cull=bv.cull(*this,r);
			if(cull){
				metrics.viewcull++;
				return;
			}
			metrics.globsrend++;
//			if(drawboundingspheres)drawboundingsphere();
			gldraw();
			for(auto g:chs)
				g->culldraw(bv);
		}
		void dotck(){
			if(tk==clk.tk){
				flf();l("same tk");
				return;
			}
			tk=clk.tk;
			tick();
		}
		virtual void tick(){
			d.set(nd);
			set(np);
			chs.splice(chs.end(),chsadd);
			for(auto g:chs)
				g->dotck();
			for(auto g:chsrm){
				chs.remove(g);
				delete g;
			}
			chsrm.clear();
			if(!ppsaved){
				pp.set(*this);
				ppsaved=false;//?
			}
	//		flf();l()<<"f("<<f<<") fi("<<fi<<") m("<<m<<") dd("<<dd<<") d("<<d<<") ("<<*this<<") dt("<<dt()<<") "<<endl;
			dd=pt(f).transl(fi).scale(1/m);//? dda
			fi.clr();
			d.transl(dd,dt());
			transl(d,dt());
//			cout<<id<<" "<<dt()<<"   "<<*this<<endl;
			a.transl(da,dt());
			np.set(*this);
			nd.set(d);
		}
		virtual void gldraw(){
			if(vb||drawaxis){
				GLfloat mx[16];
				getmxmw().togl(mx);
				glUniformMatrix4fv(shader.umxmw,1,false,mx);
			}
			if(vb)
				vb->gldraw();
			if(drawaxis)
				vbos::axis.gldraw();
			if(drawboundingspheres){
				const pt pos=posinwcs(pt());
//				flf();l()<<pos<<"   "<<r<<endl;
				GLfloat mx[16];
				mtx mw;
				mw.setsclagltrans(pt(r,r,r),pt(),pos);
				mw.togl(mx);
				glUniformMatrix4fv(shader.umxmw,1,false,mx);
				vbos::circlexy.gldraw();
			}
			for(auto g:chs)g->gldraw();
		};
		inline pt&agl(){return a;}//?
		inline pt&getd(){return d;}//?
		inline float mass()const{return m;}
		inline glob&parent()const{return g;}
		inline int getid()const{return id;}
		inline const list<glob*>chls()const{return chs;}
		inline float radius()const{return r;}
		inline glob&radius(const float r){this->r=r;return*this;}
		inline const pt&angle()const{return a;}
		inline pt&dp(){return d;}
		inline bool iscolmx()const{return bits&16;}
		inline glob&setblt(const bool b){if(b)bits|=2;else bits&=0xfffffffd;return*this;}
		inline glob&setitem(const bool b){if(b)bits|=8;else bits&=0xfffffff8;return*this;}
	protected:
		inline bool issolid()const{return bits&1;}
		inline glob&setsolid(const bool b){if(b)bits|=1;else bits&=0xfffffffe;return*this;}
		inline bool isblt()const{return bits&2;}
		inline bool iscoldetrec()const{return bits&4;}
		inline glob&setcoldetrec(const bool b){if(b)bits|=4;else bits&=0xfffffffb;return*this;}
		inline bool isitem()const{return bits&8;}
		inline glob&setcolmx(const bool b){if(b)bits|=16;else bits&=0xfffffff0;return*this;}
		//? static
		bool solvesecdegeq(const float a,const float b,const float c,float&t1,float&t2)const{
			const float pt2=2*a;
			if(pt2==0){return false;}
			const float pt1=sqrt(b*b-4*a*c);
			if(pt1!=pt1){
				flf();l()<<" nan "<<endl;
				return false;
			}
			t1=(-b-pt1)/pt2;
			t2=(-b+pt1)/pt2;
			return true;
		}
		virtual bool oncol(glob&o){//? defunc
			metrics.collisions++;
			//flf();l()<<typeid(*this).name()<<"["<<this->getid()<<"]"<<endl;
			if(!o.issolid())return true;
			const pt&p1=*this;
			const pt&u1=d;
			const pt&p2=o;
			const pt&u2=o.d;
			const float r1=radius();
			const float r2=o.radius();
			const float r0=r1+r2;
			const pt du=pt(u2,u1);
			const float a=pt(du).dot(du);
			const pt dp=pt(p2,p1);
			const float b=2*pt(dp).dot(du);
			const float c=pt(dp).dot(dp)-r0*r0;
			float t1=0,t2=0;
			if(!solvesecdegeq(a,b,c,t1,t2)){
	//			const float d=p3(p1,p2).magn();
				flf();cout<<"t1="<<t1<<" t2="<<t2<<" a="<<a<<" d="<<d<<" dr="<<r0<<endl;
				return true;// object in collision
			}
			float t=min(t1,t2);
			if(t<=-1)t=max(t1,t2);
			if(t>=0)t=min(t1,t2);
			if(t<=-1||t>=0){
				flf();l("t1=")<<t1<<" t2="<<t2<<" t="<<t<<"  u1("<<u1<<")"<<endl;
				return true;
			}
			np.set(p1).transl(u1,t);//move objects out of collision
			pt np2(p2);
			np2.transl(u2,t);
			const pt nml(np,np2,true);//collision normal
			pt vu1(nml);//velocity along normal
			vu1.scale(u1.dot(nml));
			pt vu2(nml);
			vu2.scale(u2.dot(nml));
			const float m1=m;
			const float m2=o.m;
			const float mm=1/(m1+m2);
			pt v1(u1);
			v1.transl(vu1,-1);//removes the velocity along normal
			v1.transl(vu1,(m1-m2)*mm*bf);//reflect
			v1.transl(vu2,2*m2*mm*bf);
	//		flf();l()<<"nml("<<nml<<") u1("<<u1<<") u2("<<u2<<") vu1("<<vu1<<") vu2("<<vu2<<") v1("<<v1<<") m1("<<m1<<") m2("<<m2<<")"<<endl;
			nd.set(v1);//new velocity
			flf();l()<<t<<endl;
			np.transl(nd,dt()*(-t));//calculate the rest of dt with new velocity
			return true;
		}

	protected:
		pt posinwcs(const pt&p){
			refreshmxmw();
			pt d;
			mxmw.trnsf(p,d);//? ifidentskip
			return d;
		}
		void refreshmxmw(){
			if(!&g)//skip root object with ortho norm xyzw
				return;
			if(g.mxmwtk==ptmxupdtk){
				if(mxmwpos==*this&&mxmwagl==a&&mxmwscl==getscl())
					return;
			}
			metrics.mwrfsh++;
			mxmwagl=a;
			mxmwpos=*this;
			mxmwscl=scl;
//			flf();l()<<mxmwscl<<endl;
//			mxmw.mw(mxmwpos,mxmwagl);//? cache
			mxmw.setsclagltrans(mxmwscl,mxmwagl,mxmwpos);//?cache
			if(&g.g)// skip mul with ident
				mxmw.mul(g.getmxmw());

//			mxmw=g.mxmw;
//			mxmw.mul2(mtx(mxmwpos,mxmwagl));//? cache

			mxmwtk=clk.tk;
			ptmxupdtk=g.mxmwtk;
			return;
		}
	};
	bool glob::drawboundingspheres=true;
	int glob::drawboundingspheresdetail=6;
	bool glob::drawaxis=true;

	class grid{
		pt po;
		float s;
		list<glob*>ls;
		list<glob*>lsmx;
		grid*grds[8];
		const size_t splitthresh=100;
		const int subgridlevels=4;
	public:
		grid(const float size,const pt&p=pt(0,0,-.5f))://displaced to optimize z=0
			po(p),s(size)
//			,grds({0,0,0,0,0,0,0,0})
		{
			metrics.ngrids++;
			memset(grds,0,sizeof(grds));
		}
		~grid(){metrics.ngrids--;clear();}
		void tick(){
			for(auto g:ls)
				g->dotck();
			for(auto g:lsmx)
				g->dotck();
			if(grds[0])
				for(auto gr:grds)
					if(gr)
						gr->tick();
		}
		void gldraw(){
			GLfloat mx[16];
			mtx mw;
			mw.setsclagltrans(pt(s,s,s),pt(),po);
			mw.togl(mx);
			glUniformMatrix4fv(shader.umxmw,1,false,mx);
			vbos::squarexy.gldraw();
			//? sphere in viewpyr check
	//		const GLbyte c=(GLbyte)(po.gety()/15*127);
	//		glColor3b(0,0,c);
	//		glPushMatrix();
	//		glTranslatef(po.getx(),po.gety(),po.getz());
			//glutWireCube(s*2);
			//glutWireSphere(s,8,8);
	//		glPopMatrix();
			if(grds[0])
				for(auto gr:grds){
					gr->gldraw();
				}
		}
		void clear(){
			ls.clear();
			lsmx.clear();
			if(grds[0])
				for(auto&g:grds){
					g->clear();
					delete g;//? recycle
					g=0;
				}
		}
		void addall(const list<glob*>&ls){
			for(auto g:ls)
				putif(g,*g,g->radius());
			splitif(subgridlevels);//? splititonthefly
			//? ifallglobswhereaddedtoallsubgrids,stoprecurtion
		}
		void coldet(){
//			flf();l()<<ls.size()<<endl;
			if(!ls.empty()){
				auto i1=ls.begin();
				while(true){
					auto i2=ls.rbegin();
					if(*i1==*i2)
						break;
					glob&g1=*(*i1);
					do{	glob&g2=*(*i2);
						g1.coldet(g2);
						i2++;
					}while(*i1!=*i2);
					i1++;
				}
				if(!lsmx.empty())
					for(auto g1:ls)
						for(auto g2:lsmx)
							g1->coldet(*g2);
			}
			if(grds[0])
				for(auto g:grds)
					g->coldet();
		}
		void culldraw(const bvol&bv){
			const int c=bv.cull(po,s*1.41f);//? radius
			if(c){
				metrics.gridsculled++;
				return;
			}
			metrics.gridsrend++;
			for(auto g:ls){
				g->culldraw(bv);
			}
			for(auto g:lsmx){
				g->culldraw(bv);
			}
			if(grds[0])
				for(auto&g:grds)
						g->culldraw(bv);
		}
	private:
		bool putif(glob*g,const pt&p,const float r){
			if((p.getx()+s+r)<po.getx())return false;
			if((p.getx()-s-r)>po.getx())return false;
			if((p.getz()+s+r)<po.getz())return false;
			if((p.getz()-s-r)>po.getz())return false;
			if((p.gety()+s+r)<po.gety())return false;
			if((p.gety()-s-r)>po.gety())return false;
			if(g->iscolmx()){
				lsmx.push_back(g);
			}else
				ls.push_back(g);
			return true;
		}
		bool splitif(const int nrec){
			if((ls.size()+lsmx.size())<splitthresh)
				return false;
			if(nrec==0)
				return false;
			const float ns=s/2;
			grds[0]=new grid(ns,pt(po).transl(-ns,ns,-ns));//? recycle
			grds[1]=new grid(ns,pt(po).transl( ns,ns,-ns));
			grds[2]=new grid(ns,pt(po).transl(-ns,ns, ns));
			grds[3]=new grid(ns,pt(po).transl( ns,ns, ns));

			grds[4]=new grid(ns,pt(po).transl(-ns,-ns,-ns));
			grds[5]=new grid(ns,pt(po).transl( ns,-ns,-ns));
			grds[6]=new grid(ns,pt(po).transl(-ns,-ns, ns));
			grds[7]=new grid(ns,pt(po).transl( ns,-ns, ns));

			for(auto g:grds){
				for(auto o:ls)
					g->putif(o,*o,o->radius());
				for(auto o:lsmx)
					g->putif(o,*o,o->radius());
				g->splitif(nrec-1);
			}
			ls.clear();
			lsmx.clear();
			return true;
		}
	};


	class wold:public glob{
//		static wold wd;
		float t=0;
	public:
		wold(const float r=1):
			glob(*(glob*)0,pt(),pt(),r),
			t(0),
			grd(r),
//			kb(0),
			drawaxis(false),
			drawgrid(true),
			hidezplane(false),
			coldetbrute(false),
			coldetgrid(true)
		{bits|=1;}
		grid grd;
//		keyb*kb;
		bool drawaxis,drawgrid,hidezplane,coldetbrute,coldetgrid;
//		inline static wold&get(){return wd;}
//		inline float gett()const{return t;}
//		inline void applyg(p3&dd)const{dd.transl(0,-9.82f,0);}
//		void glload(){}
//		keyb&keyb()const{return*kb;}
	//	void gldraw(){
	////		flf();l("draw world");
	////		glCullFace(GL_FRONT);
	////		glColor3b(0,0,0);
	////		glutSolidSphere(radius(),20,20);
	//		glDisable(GL_LIGHTING);
	//		glDisable(GL_CULL_FACE);
	//		const float r=radius();
	//		if(drawgrid&&coldetgrid)
	//			grd.gldraw();
	//		if(drawaxis){
	//			//glPolygonOffset
	//			glBegin(GL_LINE_STRIP);
	//
	//			glColor3b(127,0,0);
	//			glVertex3f(0,0,0);
	//
	//			glColor3b(127,0,0);
	//			glVertex3f(r,0,0);
	//			glVertex3f(0,0,0);
	//
	//			glColor3b(0,127,0);
	//			glVertex3f(0,0,0);
	//
	//			glColor3b(0,127,0);
	//			glVertex3f(0,r,0);
	//
	//			glColor3b(0,0,127);
	//			glVertex3f(0,0,0);
	//
	//			glColor3b(0,0,127);
	//			glVertex3f(0,0,r);
	//			glEnd();
	//		}
	//		if(!hidezplane){
	//			glPushMatrix();
	//	//		glTranslatef(0,0,01f);
	//			const float a=float(sin(.1*t));
	//			glColor3f(a,a,a);
	//			glBegin(GL_TRIANGLE_FAN);
	//			glVertex2f(0,0);
	//			glVertex2f(r,0);
	//			const float dtr=3.14159f/180;
	//			const int di=360/24/2/2;
	//			for(int i=di;i<=360;i+=di){
	//				const float rd=i*dtr;
	//				glVertex3f(r*cos(rd),0,r*sin(rd));
	//			}
	//			glEnd();
	//			glPopMatrix();
	//		}
	//		glEnable(GL_LIGHTING);
	//		glEnable(GL_CULL_FACE);
	//	}
		void tick(){
	//		clk::timerrestart();
			glob::tick();

//			chs.splice(chs.end(),chsadd);
//			for(auto g:chs)g->dotck();
//			for(auto g:chsrm){chs.remove(g);delete g;}

	//		metrics.dtupd=clk::timerdt();

	//		clk::timerrestart();
			t+=dt();
			tmr t;
			grd.clear();
			grd.addall(chls());
			metrics.dtgrdput=t.dt();
			if(coldetgrid){
				grd.coldet();
			}
			metrics.dtcoldetgrd=t.dt();

	//		clk::timerrestart();
			if(coldetbrute){
				auto i1=getchs().begin();//? chls() givescrash,const?
				while(true){
					auto i2=getchs().rbegin();
					if(*i1==*i2)
						break;
					glob&g1=*(*i1);
					do{
						glob&g2=*(*i2);
						g1.coldet(g2);
						i2++;
					}while(*i1!=*i2);
					i1++;
				}
			}
	//		metrics.dtcoldetbrute=clk::timerdt();
		}
	};
	static wold wd;
	class keyb{
	public:
		virtual~keyb(){};//?
		virtual void onkeyb(const int c=0,const bool pressed=false,const int x=0,const int y=0)=0;
	};

	class vehicle:public glob{
		float fwdbckrate;
		float straferate;
		float turnrate;
		float rocketforce;
		float rocketfuelburnrate;
		float smallflapimpulseforce;
		float smallflapfuelburn;
		float bigflapimpulseforce;
		float bigflapfuelburn;
		float leapimpulseforce;
		float leapfuelburn;
		float flapperyrecoveryrate;
		float flapperymax;
		float rocketryrecoveryrate;
		float rocketrymax;
		float initflappery;
		float initrocketry;
		float flappery;
		float rocketry;
		int items=0;
		int player=0;
		mtx mxv;
	public:
		vehicle(glob&g,const pt&p=pt(),const pt&a=pt(),const float r=1,const float density_gcm3=1,const float bounciness=.5f):
			glob(g,p,a,r,density_gcm3,bounciness),
			fwdbckrate(.001f),
			straferate(.001f),
			turnrate(180),
			rocketforce(150*mass()),
			rocketfuelburnrate(6),
			smallflapimpulseforce(7*mass()),
			smallflapfuelburn(.4f),
			bigflapimpulseforce(15*mass()),
			bigflapfuelburn(1),
			leapimpulseforce(17*mass()),
			leapfuelburn(2),
			flapperyrecoveryrate(3),
			flapperymax(1),
			rocketryrecoveryrate(1),
			rocketrymax(3),
			initflappery(0),
			initrocketry(0),
			flappery(initflappery),
			rocketry(initrocketry),
			items(0),
			player(0)
		{}
		virtual~vehicle(){}
		virtual void tick(){
			flappery+=dt(flapperyrecoveryrate);
			if(flappery>flapperymax)flappery=flapperymax;
			rocketry+=dt(rocketryrecoveryrate);
			if(rocketry>rocketrymax)rocketry=rocketrymax;
			glob::tick();
		}
		virtual void handlekeys(){
			pp.set(*this);ppsaved=true;
			mxv.mw(*this,agl());
			if(hdlkeydn('w')){fi.transl(mxv.zaxis().sety(0).norm().scale(-fwdbckrate));}
			if(hdlkeydn('s')){fi.transl(mxv.zaxis().sety(0).norm().scale(fwdbckrate));}
			if(hdlkeydn('d')){fi.transl(mxv.xaxis().scale(straferate));}
			if(hdlkeydn('a')){fi.transl(mxv.xaxis().scale(-straferate));}
			if(hdlkeydn('j')){agl().transl(0,dt(turnrate),0);}
			if(hdlkeydn('l')){agl().transl(0,-dt(turnrate),0);}
			if(hdlkeydn(',')&&rocketry>0){f.transl(0,dt(rocketforce),0);rocketry-=dt(rocketfuelburnrate);}else{f.set(0,0,0);}
			if(hdlkeydn('b')){d.set(0,0,0);nd.set(0,0,0);}//? use fi and f=ma
			if(hdlkeydn('t')){transl(mxv.yaxis(),dt(fwdbckrate));}
			if(hdlkeydn('g')){transl(mxv.yaxis(),dt(-fwdbckrate));}
			if(hdlkeytg('i')){if(flappery>0){fi.transl(0,smallflapimpulseforce,0);flappery-=smallflapfuelburn;}}
			if(hdlkeytg('k')){if(flappery>0){fi.transl(0,bigflapimpulseforce,0);flappery-=bigflapfuelburn;}}
			if(hdlkeytg('m')){if(flappery>0){fi.transl(mxv.zaxis().neg().negy().scale(leapimpulseforce));flappery-=leapfuelburn;}}
			if(hdlkeytg('x')){agl().transl(7,0,0);}
			if(hdlkeytg('c')){agl().transl(-7,0,0);}
			if(hdlkeytg('z')){agl().setx(0);}
		}
		void mouseclk(const int button,const int state,int x,const int y){
			cout<<"mouseclk: "<<button<<" "<<state<<" "<<x<<" "<<y<<endl;
			/*sts<<"mousclk("<<state<<","<<button<<",["<<x<<","<<y<<",0]");*/
		}
		void mousemov(const int x,const int y){
			cout<<"mousemov: "<<x<<" "<<y<<endl;
			/*sts<<"mousmov("<<x<<","<<y<<")";*/
		}
		inline int getplayer()const{return player;}
		inline void setplayer(const int i){player=i;}
		inline const mtx&getmxv()const{return mxv;}
		inline float getflappery()const{return flappery;}
		inline float getrocketry()const{return rocketry;}
		inline float getitems()const{return items;}
	protected:
		bool hdlkeydn(const char key){
			const int i=net.keyix(key);
			if(!i)return false;
			const int s=net.keys[player*net.nkeys+i];
			if(s==0)return false;
			if(s==2)return true;
			if(s!=1)throw signl(2,"unknownstate");
			net.keys[player*net.nkeys+i]=2;//?
			return true;
		}
		bool hdlkeytg(const char key){
			const int i=net.keyix(key);
			if(!i)return false;
			const int s=net.keys[player*net.nkeys+i];
			if(s==0)return false;
			if(s==2)return false;
			if(s!=1)return false;
			net.keys[player*net.nkeys+i]=2;//?
			return true;
		}
	private:
	};

	class windo:public vehicle{
		bool dodrawhud,gamemode,fullscr,viewpointlht,drawshadows;
		float zoom;
		int wi,hi;
//		float firereload;
		tmr drawtmr;
		bool dopersp;
	public:
		windo(glob&g=wd,const pt&p=pt(),const pt&a=pt(),const float r=.1f,const int width=1024,const int height=512,const float zoom=1.5):
			vehicle(g,p,a,r,.25f),
			dodrawhud(false),
			gamemode(false),
			fullscr(false),
			viewpointlht(false),
			drawshadows(true),
			zoom(zoom),
			wi(width),
			hi(height),
//			firereload(0),
			dopersp(true)
		{}
		void resize(const int width,const int height){
//			cout<<"reshape "<<width<<" x "<<height;
			wi=width;hi=height;
			glViewport(0,0,wi,hi);
	//		sts<<"reshape("<<wi<<"x"<<hi<<")";wi=width;hi=height;
		}
		void drawframe(){
			glClearColor(1,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			mtx mwv;
			mwv.wv(*this,agl());//? cache
			GLfloat mx[16];
			mwv.togl(mx);
			glUniformMatrix4fv(shader.umxwv,1,false,mx);
			glUniform1i(shader.udopersp,dopersp);
			bvol bv;
//			bv.addplane(new plane(pt(),pt(1,0,0)));
//			bv.addplane(new plane(pt(),pt(0,1,0)));
//			bv.addplane(new plane(pt(),pt(0,0,1)));
//			wd.culldraw(bv);
			wd.grd.culldraw(bv);
			if(wd.drawgrid)
				wd.grd.gldraw();
		}
	//	void drawframe(){
	//		const float freq=drawtmr.dt();
	//		drawtmr.restart();
	//		cout<<"\nframe("<<metrics.frames++<<") last frame dt="<<freq<<" s  fps ~"<<1/freq;
	//		clk::timerrestart();
	//		const GLfloat lhtpos[]={getx(),gety()+radius()*2,getz(),1};
	////		GLfloat mflhtproj[16];
	//		GLfloat mxtexlht[16];
	//		GLfloat mxlhtwv[16];
	//
	//		glEnable(GL_CULL_FACE);
	//		glClearDepth(1);
	//		glEnable(GL_DEPTH_TEST);
	//		if(drawshadows){
	//			if(!gltexshadowmap){
	//				glActiveTexture(GL_TEXTURE2);
	//				glGenTextures(1,&gltexshadowmap);
	//				glBindTexture(GL_TEXTURE_2D,gltexshadowmap);
	//				glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,shadowmapsize,shadowmapsize,0,GL_DEPTH_COMPONENT,GL_FLOAT,0);
	//				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	//				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	////				const GLfloat bordercolor[]={1,0,0,0};
	//				const GLfloat bordercolor[]={0,0,0,0};
	//				glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,bordercolor);
	//			}
	////			glMatrixMode(GL_PROJECTION);
	////			glLoadIdentity();
	////			glGetFloatv(GL_PROJECTION_MATRIX,mflhtproj);
	////			glMatrixMode(GL_MODELVIEW);
	////			glLoadIdentity();
	//			const p3 lhtlookat=p3(getmxv().zaxis().neg().scale(10)).transl(*this);
	////			gluLookAt(lhtpos[0],lhtpos[1],lhtpos[2], lhtlookat.getx(),lhtlookat.gety(),lhtlookat.getz(), 0,1,0);
	////			glGetFloatv(GL_MODELVIEW_MATRIX,mxlhtwv);
	////			glLoadIdentity();
	////			gluPerspective(80,1,.001,1000);
	////			gluLookAt(lhtpos[0],lhtpos[1],lhtpos[2], lhtlookat.getx(),lhtlookat.gety(),lhtlookat.getz(), 0,1,0);
	////			glGetFloatv(GL_MODELVIEW_MATRIX,mxtexlht);
	//			glCullFace(GL_FRONT);
	//			if(!viewpointlht)
	//				glColorMask(0,0,0,0);
	//			else
	//				glClear(GL_COLOR_BUFFER_BIT);
	//			glClear(GL_DEPTH_BUFFER_BIT);
	//			glViewport(0,0,shadowmapsize,shadowmapsize);
	//			glUseProgram(0);//? depthbuffershader
	//
	//			const p3 lhtcoord(lhtpos[0],lhtpos[1],lhtpos[2]);
	//			const p3 zinv=p3(mxlhtwv[2],mxlhtwv[6],mxlhtwv[10]);
	//			const p3n backplane(lhtcoord,zinv);//? viewfurst
	//			const p3n cullplanes[]{backplane};
	//			const bvol bv(1,cullplanes);
	//			wold::get().gldraw();//? y culled
	//			wold::get().grd.culldraw(bv);//? cull viewfurst
	//			clk::tk++;//? increase frame instead of clear rendered bit
	//			if(viewpointlht)
	//				return;
	//			glActiveTexture(GL_TEXTURE2);
	//			glBindTexture(GL_TEXTURE_2D,gltexshadowmap);
	//			glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,shadowmapsize,shadowmapsize);//? fbo
	//			glColorMask(1,1,1,1);
	//			glUseProgram(shader::prog);//? glprog.use()
	//		}
	//		glClearColor(.1f,.1f,.5f,1);
	//		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//		glCullFace(GL_BACK);
	//		glViewport(0,0,wi,hi);
	//		glMatrixMode(GL_PROJECTION);
	//		glLoadIdentity();
	//		const float viewangle_deg=45*zoom;
	//		gluPerspective(viewangle_deg,(GLdouble)wi/hi,.001f,1000);
	//		const p3 lookat=p3(getmxv().zaxis().neg()).transl(*this);
	//		gluLookAt(getx(),gety(),getz(), lookat.getx(),lookat.gety(),lookat.getz(), 0,1,0);
	////		GLfloat mfcamproj[16];glGetFloatv(GL_PROJECTION_MATRIX,mfcamproj);
	//
	//		glMatrixMode(GL_MODELVIEW);
	//		glLoadIdentity();
	//		gluLookAt(getx(),gety(),getz(), lookat.getx(),lookat.gety(),lookat.getz(), 0,1,0);
	//		GLfloat mf[16];glGetFloatv(GL_MODELVIEW_MATRIX,mf);
	//		glLoadIdentity();
	//
	//		if(drawshadows){
	//			glActiveTexture(GL_TEXTURE2);
	//			glBindTexture(GL_TEXTURE_2D,gltexshadowmap);
	//			//glGenerateMipmap(GL_TEXTURE_2D);
	//			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	//
	//			glMatrixMode(GL_TEXTURE);
	//			const GLfloat bias[]={.5f,0,0,0, 0,.5f,0,0, 0,0,.5f,0, .5f,.5f,.5f,1};
	//			glLoadMatrixf(bias);
	//			glMultMatrixf(mxtexlht);
	//			glMatrixMode(GL_MODELVIEW);
	//		}
	//
	//
	//		const p3 xinv=p3(mf[0],mf[4],mf[8]);
	//		const p3 yinv=p3(mf[1],mf[5],mf[9]);
	//		const p3 zinv=p3(mf[2],mf[6],mf[10]);
	//		const p3n backplane(*this,zinv);//? viewfurst
	//		const float viewangle_rad=degtorad(viewangle_deg);
	//		const float scrdst=(wi/2)/tan(viewangle_rad)/zoom;//?
	//		const float ww=wi/4;
	//		const float hh=hi/4;
	//		p3 ptr(*this);
	//		ptr.transl(p3(zinv).neg().scale(scrdst));
	//		ptr.transl(p3(xinv).scale(ww));
	//		ptr.transl(p3(yinv).scale(hh));
	//		p3 pbr(*this);
	//		pbr.transl(p3(zinv).neg().scale(scrdst));
	//		pbr.transl(p3(xinv).scale(ww));
	//		pbr.transl(p3(yinv).scale(-hh));
	//		p3 pbl(*this);
	//		pbl.transl(p3(zinv).neg().scale(scrdst));
	//		pbl.transl(p3(xinv).scale(-ww));
	//		pbl.transl(p3(yinv).scale(-hh));
	//		p3 ptl(*this);
	//		ptl.transl(p3(zinv).neg().scale(scrdst));
	//		ptl.transl(p3(xinv).scale(-ww));
	//		ptl.transl(p3(yinv).scale( hh));
	//		//? farzplane
	//		p3 rightplanenml(*this,p3());
	//		rightplanenml.vecprod(pbr,ptr).norm();
	//		const p3n rightplane(*this,rightplanenml);
	//		p3 leftplanenml(*this,p3());
	//		leftplanenml.vecprod(ptl,pbl).norm();
	//		p3n leftplane(*this,leftplanenml);
	//		p3 topplanenml(*this,p3());
	//		topplanenml.vecprod(ptr,ptl).norm();
	//		const p3n topplane(*this,topplanenml);
	//		p3 btmplanenml(*this,p3());
	//		btmplanenml.vecprod(pbl,pbr).norm();
	//		const p3n btmplane(*this,btmplanenml);
	//		metrics.viewcull=metrics.globsrend=metrics.gridsculled=metrics.gridsrend=0;
	//		const p3n cullplanes[]{backplane,rightplane,leftplane,topplane,btmplane};
	//		const bvol bv(5,cullplanes);
	//		wold::get().gldraw();//? yisculled?
	//		wold::get().grd.culldraw(bv);//. rendleftrighti
	//		metrics.dtrend=clk::timerdt();
	//		if(dodrawhud){
	//			glMatrixMode(GL_MODELVIEW);
	//			glLoadIdentity();
	//			glMatrixMode(GL_PROJECTION);
	//			glLoadIdentity();
	//			glOrtho(0,wi,0,hi,0,1);
	//			glColor3b(0x7f,0x00,0x00);
	//			drawhud();
	//		}
	//		cout<<flush;
	//	}
	//	void gldraw(){
	//		glColor3f(1,0,0);
	//		glutSolidSphere(radius(),20,20);
	//	}
		void handlekeys(){
			if(hdlkeytg('1')){glob::drawboundingspheres=!glob::drawboundingspheres;}
			if(hdlkeytg('2')){wd.drawaxis=!wd.drawaxis;}
			if(hdlkeytg('3')){wd.drawgrid=!wd.drawgrid;}
			if(hdlkeytg('4')){wd.hidezplane=!wd.hidezplane;}
	//		if(hdlkeytg('5')){wold::get().coldetgrid=!wold::get().coldetgrid;}
			if(hdlkeytg('5')){drawshadows=!drawshadows;}
			if(hdlkeytg('6')){viewpointlht=!viewpointlht;}
			if(hdlkeytg('7')){glob::drawaxis=!glob::drawaxis;}
			if(hdlkeytg('8')){dopersp=!dopersp;}
			if(hdlkeydn(' ')){fire();}
			if(hdlkeytg(9)){togglehud();}// tab
			if(hdlkeytg('y')){zoom-=.1;}
			if(hdlkeytg('h')){zoom+=.1;}
			if(hdlkeytg('0')){togglefullscr();return;}
			if(hdlkeytg(27)){if(fullscr)togglefullscr();cout<<endl;exit(0);}// esc
			vehicle::handlekeys();
		}
		inline bool isgamemode()const{return gamemode;}
		inline bool isfullscreen()const{return fullscr;}
		inline int width()const{return wi;}
		inline int height()const{return hi;}
	private:
		inline windo&togglehud(){dodrawhud=!dodrawhud;return*this;}
		void togglefullscr(){
	//		if(gamemode)
	//			return;
	//		fullscr=!fullscr;
	//		if(fullscr){
	//			wiprv=wi;hiprv=hi;
	//			glutFullScreen();
	//			glutSetCursor(GLUT_CURSOR_NONE);
	//		}else{
	//			glutReshapeWindow(wiprv,hiprv);
	//			glutSetCursor(GLUT_CURSOR_INHERIT);
	//		}
		}
		void fire(){
			const float r=.01f;
			const float s=.5f;
			glob*g=new glob(wd,pt(rnd(-s,s),rnd(-s,s),0),pt(),r,1,1,vbos::spritexy_);
			const float d=.05f;
			g->dpos(pt(rnd(-d,d),rnd(-d,d),0),pt(0,0,rnd(-180,180)));

//			nd.set(getmxv().zaxis().neg().scale(dt()));
	////		firereload+=dt(60);if(firereload>1)firereload=1;
	////		if(firereload<1)return;
	////		firereload-=1;
	//		const p3 lv=getmxv().zaxis().neg();
	//		const float r=.02f;
	//		const float v=.2f;
	//		p3 vv=p3(d).transl(p3(lv).scale(v));
	//		const float sprd=r/5;
	//		const float sx=rnd(-sprd,sprd);
	//		const float sy=rnd(-sprd,sprd);
	//		const float sz=rnd(-sprd,sprd);
	//		vv.transl(sx,sy,sz);
	////		globx&o=*new obball(wold::get(),lv.scale(radius()+r).transl(sx,sy,sz).transl(*this),r);
	//		glob&o=*new objects::ball(wold::get(),*this,r,4,1,1,0xffff0000);
	//		o.getd().set(vv);
	//		o.nd.set(vv);//?
	//		o.setblt(true);
		}
	//	void pl(const char*text,const GLfloat y=0,const GLfloat x=0,const GLfloat linewidth=1,const float scale=1){
	////		const char*cp=text;
	////		glPushMatrix();
	////		glTranslatef(x,y,0);
	////		glScalef(scale,scale,0);
	////		glLineWidth(linewidth);
	////		for(;*cp;cp++)
	////			glutStrokeCharacter(GLUT_STROKE_ROMAN,*cp);
	////			glutStrokeString(GLUT_STROKE_MONO_ROMAN,text);
	////		glPopMatrix();
	//	}
		void drawhud(){
	//		const int dy=hi>>5;int y=-dy;

//			timeval tv;gettimeofday(&tv,0);
//			const tm&t=*localtime(&tv.tv_sec);
			ostringstream oss;
	//		oss<<setprecision(2)<<fixed;
//			oss<<t.tm_hour<<":"<<":"<<t.tm_min<<":"<<t.tm_sec<<"."<<tv.tv_usec/1000<<"    t("<<wold::get().gett()<<")";
	//		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

			oss.str("");
	//		oss<<setprecision(1);
			oss<<"p("<<*this<<") d("<<this->d<<") fi("<<this->fi<<") a("<<angle()<<") zoom("<<zoom<<")";
	//		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

			oss.str("");
	//		oss<<setprecision(2);
			oss<<"frame("<<metrics.frames<<") globs("<<metrics.globs<<") p3s("<<metrics.p3s<<") m3s("<<metrics.m3s<<") rays("<<metrics.rays<<")";
	//		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

			oss.str("");
	//		oss<<setprecision(4);
//			oss<<"upd("<<metrics.dtupd<<")s rayone("<<metrics.rayone<<")s draw("<<metrics.dtrend<<")s    "<<((int)(metrics.globs/(metrics.dtrend?metrics.dtrend:1))>>10)<<"Kglobs/s    rendfpsest("<<(1/(metrics.dtrend+metrics.dtupd+metrics.dtcoldetgrd))<<")f/s";
	//		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

			oss.str("");
	//		oss<<setprecision(4);
//			oss<<"coldet("<<(wold::get().coldetgrid?"grid":"")<<" "<<(wold::get().coldetbrute?"brute":"")<<") ngrids("<<metrics.ngrids<<") grid("<<metrics.dtcoldetgrd<<")s  "<<(((long long int)(metrics.globs/(wold::get().coldetgrid?metrics.dtcoldetgrd:metrics.dtcoldetbrute)))>>10)<<"Kglobs/s   brutedt("<<metrics.dtcoldetbrute<<")s";
	//		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

			oss.str("");
			oss<<"sphcolsdet("<<metrics.coldetsph<<") sphcols("<<metrics.collisions<<")"<<" mxrfsh("<<metrics.mwrfsh<<")"<<" mvmul("<<metrics.mpmul<<") mmmul("<<metrics.mmmul<<") ";
	//		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

			oss.str("");
	//		oss<<setprecision(4);
			oss<<"gridscull("<<metrics.gridsculled<<") gridsrend("<<metrics.gridsrend<<") cullview("<<metrics.viewcull<<") globstorend("<<metrics.globsrend<<")";
	//		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

			oss.str("");
	//		oss<<setprecision(4);
			oss<<"player("<<getplayer()<<") dtnet("<<metrics.dtnet<<")";
	//		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

			oss.str("");
	//		oss<<setprecision(1);
			oss<<"flappery("<<getflappery()<<") "<<"rocketry("<<getrocketry()<<") "<<"items("<<getitems()<<")";
	//		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

	//		y+=dy;pl(sts.str().c_str(),y,0,1,.1f);
	//		sts.str("");
		}
	};
	static windo*wn;

	void GLFWCALL onkeyb(const int key,const int pressed){
		cout<<"keyboard key "<<key<<"   "<<pressed<<endl;
		net.onkeyb((const char)key,pressed,0,0);
//		if(wn)
//			wn->onkeyb(key,pressed,0,0);
	}
	void GLFWCALL onresize(const int width,const int height){
//		cout<<"window resize "<<width<<" x "<<height<<endl;
		if(wn)
			wn->resize(width,height);
	}
	void init(){
		if(!glfwInit())throw signl(1,"could not init glsig");
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,3);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR,2);
		glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
		glfwOpenWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
		if(!glfwOpenWindow(512,512,8,8,8,8,32,0,GLFW_WINDOW))return;
		glfwSwapInterval(0);
		glfwEnable(GLFW_STICKY_KEYS);
		glfwSetWindowSizeCallback(onresize);
		glfwSetKeyCallback(onkeyb);
		cout<<"reray"<<endl;
		cout<<"   opengl: "<<glGetString(GL_VERSION)<<endl;
	//	printf("sizeofs\n");
		shader.init();

//		glEnable(GL_LINE_SMOOTH);
//		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		if(glGetError()!=GL_NO_ERROR)throw signl(1,"opengl is in error state");
		printf(": %8s : %-4lu :\n","bool",sizeof(bool));
		printf(": %8s : %-4lu :\n","char",sizeof(char));
		printf(": %8s : %-4lu :\n","int",sizeof(int));
		printf(": %8s : %-4lu :\n","float",sizeof(float));
		printf(": %8s : %-4lu :\n","double",sizeof(double));
		printf(": %8s : %-4lu :\n","long",sizeof(long));
		printf(": %8s : %-4lu :\n","long long int",sizeof(long long));
		printf(": %8s : %-4lu :\n","pt",sizeof(pt));
		printf(": %8s : %-4lu :\n","mtx",sizeof(mtx));
		printf(": %8s : %-4lu :\n","bvol",sizeof(bvol));
		printf(": %8s : %-4lu :\n","glob",sizeof(glob));
		printf(": %8s : %-4lu :\n","vbo",sizeof(vbo));
//		printf(": %8s : %-4lu :\n","dbox",sizeof(dbox));
		vbo_.glload();
		vbos::axis.glload();
		vbos::circlexy.glload();
		vbos::squarexy.glload();
		vbos::spritexy_.glload();
	}
	void hello(){cout<<"Hello from thread "<<endl;}
	void run(){
//		std::thread thd(hello);
//		thd.join();
		long frm=0;
		tmr t,t1,t2;
		printf(": %5s : %5s : %5s : %4s : %5s : %5s : %5s : %5s : %5s : %8s : %8s : %8s : %8s : %8s : %8s :\n","frame","globs","rend","fps","grids","cull","chk","cols","mxmul","dtdrw","dttck","dtswp","dt","dtgrdpt","dtcoldet");
	//	cout<<"frame"<<" "<<"dt      "<<" "<<"dt     "<<" "<<"globs"<<" "<<"globsrend"<<"\n";
		while(glfwGetWindowParam(GLFW_OPENED)){
			if(glGetError()!=GL_NO_ERROR)throw signl(0,"opengl in error");
			if(net.nplayers>1){
				net.sendkeys();
				net.reckeys();//? netlag
			}
			frm++;
			clk.tk++;
			tmr t3;
			if(wn)
				wn->drawframe();
			const float drawframedt=t3.dt();
			wd.dotck();
			const float tickdt=t3.dt();
			if(wn)
				glfwSwapBuffers();
			const float swapbufsdt=t3.dt();
			if(net.nplayers<2){
				clk.dt=t.dt();//? clk.set(t.dt())
				clk.fps=(int)(1/clk.dt);
			}else{
				net.reckeys();
			}
			if(wn)
				wn->handlekeys();
			printf(": %5ld : %5d : %5d : %4d : %5d : %5d : %5d : %5d : %5d : %8f : %8f : %8f : %8f : %8f : %8f :\r",frm,metrics.globs,metrics.globsrend,clk.fps,metrics.ngrids,metrics.gridsculled,metrics.coldetsph,metrics.collisions,metrics.mmmul,
					drawframedt,tickdt,swapbufsdt,clk.dt,metrics.dtgrdput,metrics.dtcoldetgrd);
			metrics.globsrend=metrics.mmmul=metrics.gridsculled=metrics.coldetsph=metrics.collisions=0;
		}
		cout<<endl<<frm/t1.dt()<<endl;
	}

}
#endif


using namespace dbox;
#include <string>
#include <fstream>
namespace app{
	class vboobj:public vbo{
		int nverts=0;
	public:
		vboobj():nverts(0){}
		virtual inline const char*name()const{return "vboobj";}
		virtual inline const char*objfilepath()const{return "untitled0.obj";}
		inline int elemtype()const{return 5;}
		void init0(){
			string line;
			ifstream infile(objfilepath());
			if(!infile.is_open())throw signl(1,objfilepath());
			cout<<"  "<<objfilepath()<<endl;
			while(getline(infile,line)){
				if(line.at(0)=='v'){
					nverts++;
				}
			}
		}
		int nvertices()const{return nverts;}
		virtual GLsizei nindices(){return 0;}
		virtual void vertices(float fa[])const{
			string line;
			ifstream infile(objfilepath());
			int nverts=0;
			int j=0;
			while(getline(infile,line)){
				if(line.at(0)=='v'){
					istringstream iss(line);
					float x,y,z;
					string tp;
					if(!(iss>>tp>>x>>y>>z)){
						break;
					}
					fa[j++]=x;
					fa[j++]=y;
					fa[j++]=z;

					fa[j++]=1;//r
					fa[j++]=1;//g
					fa[j++]=1;//b
					fa[j++]=1;//a

					fa[j++]=0;//s
					fa[j++]=0;//t

					nverts++;
				}
			}
		}
	}vboobj;

	class vbomonkey:public vboobj{
		inline const char*name()const{return "vbomonkey";}
		inline const char*objfilepath()const{return "untitled0.obj";}
	}vbomonkey;

	class objobj:public glob{
	public:
		objobj(glob&g=wd,const pt&p=pt(),const pt&a=pt(),const float r=1,const float density_gcm3=1,const float bounciness=1,vbo&vb=vboobj):
			glob(g,p,a,r,density_gcm3,bounciness,vb)
		{}
	};

	class objaxis:public glob{
	public:
		objaxis(glob&g=wd,const pt&p=pt(),const pt&a=pt(),const float r=1,const float density_gcm3=1,const float bounciness=1,vbo&vb=*(vbo*)0):
			glob(g,p,a,r,density_gcm3,bounciness,vb)
		{
			bits|=1;
//			issolid();
		}
	};

	class vbosprite:public vbos::spritexy{
	public:
		inline const char*name()const{return "vbosprite";}
		inline const char*teximgpath()const{return "sprite2.png";}
	}vbosprite;

	void run(const int argc,const char**argv){
//		while(argc--)puts(*argv++);

		dbox::init();
		if(argc>1){
			net.nplayers=2;
			net.host=argv[1];
			if(argc>2){
				net.port=argv[2];
			}
			net.init();
		}else
			net.init0();

		vboobj.glload();
		vbosprite.glload();
		vbomonkey.glload();

		const float s=.1f;
		glob*o=new glob(wd,pt(),pt(),s,1,0,vbomonkey);
		o->dpos(pt(),pt(0,0,1));
//		o->bits|=1;

//		glob*oo=new objobj();
//		oo->radius(s);
//		oo->setscl(pt(s,s,s));

		wn=new windo();
		wn->pos(pt(0,0,1),pt());
		wn->dpos(pt(0,0,-.01f),pt(0,0,1));

		dbox::run();
	}

//	class windobot{
//	public:
//		windo*wn;
//		void tick(){
//	//		flf();
//			wn->onkeyb('j',true,0,0);
//	//		wn->keydn('w',0,0);
//	//		wn->onkeyb(' ',true,0,0);
//			wn->onkeyb('c',true,0,0);
//		}
//	};
}
//////////////////////////////////////////////////////////////////////////
int main(const int argc,const char**argv){app::run(argc,argv);return 0;}

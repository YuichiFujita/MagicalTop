xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 101;
 10.51940;0.00000;2.81148;,
 11.16278;0.00000;-0.16763;,
 11.16278;3.35746;-0.16763;,
 10.51940;3.35746;2.81148;,
 10.78944;0.00000;-5.41944;,
 10.78917;3.35746;-5.44826;,
 -0.30544;0.00000;-6.83598;,
 -0.30570;3.35746;-6.86480;,
 -11.22444;0.00000;-5.18297;,
 -11.37241;3.35746;-5.21031;,
 -11.48683;0.00000;0.07556;,
 -11.48683;3.35746;0.07556;,
 -10.95123;0.00000;3.04201;,
 -10.95123;3.35746;3.04201;,
 -8.74561;3.35746;4.65400;,
 -8.69924;0.00000;4.69628;,
 -0.18767;0.00000;6.00634;,
 -0.18767;3.35746;6.00634;,
 8.34378;3.35746;4.47051;,
 8.29818;0.00000;4.51378;,
 10.51940;0.00000;2.81148;,
 10.51940;3.35746;2.81148;,
 11.16278;14.09328;-0.16763;,
 10.51940;14.09328;2.81148;,
 10.78952;14.09328;-5.40967;,
 -0.30535;14.09328;-6.82621;,
 -11.37206;14.09328;-5.17172;,
 -11.48683;14.09328;0.07556;,
 -10.95123;14.09328;3.04201;,
 -8.91160;14.09328;4.62777;,
 -0.18767;14.09328;6.00634;,
 8.50926;14.09328;4.44073;,
 10.51940;14.09328;2.81148;,
 11.16278;21.97110;-0.16763;,
 10.51940;21.97110;2.81148;,
 10.79191;21.97110;-5.15019;,
 -0.30297;21.97110;-6.56672;,
 -11.36969;21.97110;-4.91224;,
 -11.48683;21.97110;0.07556;,
 -10.95123;21.97110;3.04201;,
 -8.86491;21.97110;4.63515;,
 -0.18767;21.97110;6.00634;,
 8.46272;21.97110;4.44911;,
 10.51940;21.97110;2.81148;,
 13.52103;27.48468;-0.19125;,
 12.85396;27.48468;1.74128;,
 10.51940;27.48468;2.81148;,
 10.79937;26.32931;-4.33585;,
 12.82046;26.73717;-2.88338;,
 -0.29550;26.32931;-5.75239;,
 -11.36221;26.32931;-4.09790;,
 -13.89928;27.48468;0.09972;,
 -13.55838;26.90646;-1.99172;,
 -10.95123;27.48468;3.04201;,
 -13.39461;27.48468;1.55438;,
 -8.96753;27.48468;4.61893;,
 -0.18767;27.48468;6.00634;,
 8.56501;27.48468;4.43069;,
 10.51940;27.48468;2.81148;,
 11.16278;0.00000;-0.16763;,
 10.51940;0.00000;2.81148;,
 -0.24317;-3.65154;-0.04516;,
 10.78944;0.00000;-5.41944;,
 -0.30544;0.00000;-6.83598;,
 -11.22444;0.00000;-5.18297;,
 -11.48683;0.00000;0.07556;,
 -10.95123;0.00000;3.04201;,
 -0.18767;0.00000;6.00634;,
 -8.69924;0.00000;4.69628;,
 8.29818;0.00000;4.51378;,
 13.52103;27.48468;-0.19125;,
 3.63527;27.46974;-0.01393;,
 12.85396;27.48468;1.74128;,
 10.51940;27.48468;2.81148;,
 3.13883;27.46828;1.50889;,
 12.82046;26.73717;-2.88338;,
 10.79937;26.32931;-4.33585;,
 3.14387;27.17568;-1.87702;,
 -0.29550;26.32931;-5.75239;,
 -0.23641;26.93332;-3.41067;,
 -11.36221;26.32931;-4.09790;,
 -3.40005;27.23323;-1.68974;,
 -3.59108;27.46969;0.06392;,
 -13.89928;27.48468;0.09972;,
 -13.55838;26.90646;-1.99172;,
 -3.34837;27.46789;1.54074;,
 -10.95123;27.48468;3.04201;,
 -13.39461;27.48468;1.55438;,
 -0.16722;27.47004;2.92267;,
 -0.18767;27.48468;6.00634;,
 -8.96753;27.48468;4.61893;,
 8.56501;27.48468;4.43069;,
 -2.14823;35.04707;2.22936;,
 -0.14454;35.04707;3.03949;,
 -0.17286;35.04707;0.00000;,
 -2.59913;35.04707;0.02643;,
 -2.20589;35.04707;-2.33443;,
 -0.19992;35.04707;-2.90318;,
 1.81653;35.04707;-2.26337;,
 2.33680;35.04707;-0.02734;,
 1.75590;35.04707;2.19863;;
 
 92;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;14,15,16,17;,
 4;18,19,20,21;,
 4;3,2,22,23;,
 4;2,5,24,22;,
 4;5,7,25,24;,
 4;7,9,26,25;,
 4;9,11,27,26;,
 4;11,13,28,27;,
 4;29,14,17,30;,
 4;31,18,21,32;,
 4;23,22,33,34;,
 4;22,24,35,33;,
 4;24,25,36,35;,
 4;25,26,37,36;,
 4;26,27,38,37;,
 4;27,28,39,38;,
 4;40,29,30,41;,
 4;42,31,32,43;,
 3;44,45,33;,
 3;45,46,33;,
 3;46,34,33;,
 3;47,48,35;,
 3;48,44,35;,
 3;44,33,35;,
 4;35,36,49,47;,
 4;36,37,50,49;,
 3;38,51,37;,
 3;51,52,37;,
 3;52,50,37;,
 3;39,53,38;,
 3;53,54,38;,
 3;54,51,38;,
 4;55,40,41,56;,
 4;57,42,43,58;,
 3;59,60,61;,
 3;62,59,61;,
 3;63,62,61;,
 3;64,63,61;,
 3;65,64,61;,
 3;66,65,61;,
 4;67,68,66,61;,
 4;60,69,67,61;,
 3;70,71,72;,
 3;72,71,73;,
 3;71,74,73;,
 3;70,75,71;,
 3;75,76,71;,
 3;76,77,71;,
 4;76,78,79,77;,
 4;78,80,81,79;,
 3;81,80,82;,
 3;82,80,83;,
 3;80,84,83;,
 3;85,82,86;,
 3;82,83,86;,
 3;83,87,86;,
 3;88,85,89;,
 3;89,85,90;,
 3;85,86,90;,
 3;73,74,91;,
 3;91,74,89;,
 3;74,88,89;,
 3;92,93,94;,
 3;95,92,94;,
 3;96,95,94;,
 3;97,96,94;,
 3;98,97,94;,
 3;99,98,94;,
 3;100,99,94;,
 3;93,100,94;,
 4;39,40,55,53;,
 4;28,29,40,39;,
 4;13,14,29,28;,
 4;12,15,14,13;,
 4;41,42,57,56;,
 4;30,31,42,41;,
 4;17,18,31,30;,
 4;16,19,18,17;,
 4;85,88,93,92;,
 4;82,85,92,95;,
 4;81,82,95,96;,
 4;79,81,96,97;,
 4;77,79,97,98;,
 4;71,77,98,99;,
 4;74,71,99,100;,
 4;88,74,100,93;;
 
 MeshMaterialList {
  1;
  92;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\MODEL\\TEXTURE\\enemyBody.jpg";
   }
  }
 }
 MeshNormals {
  102;
  0.844375;0.000524;0.535753;,
  0.997484;-0.000134;0.070889;,
  0.997495;-0.000264;-0.070742;,
  -0.011166;-0.010115;-0.999887;,
  -0.148761;-0.011534;-0.988806;,
  -0.997387;-0.011117;0.071383;,
  -0.848031;0.000531;0.529947;,
  0.010694;0.005211;0.999929;,
  0.846195;-0.000688;0.532873;,
  0.997485;-0.000039;0.070874;,
  0.997493;-0.000076;-0.070771;,
  -0.010936;-0.003260;-0.999935;,
  -0.148313;-0.003987;-0.988932;,
  -0.997178;-0.005543;0.074872;,
  -0.849837;-0.000696;0.527046;,
  0.010693;0.002606;0.999939;,
  0.849388;-0.000570;0.527769;,
  0.997519;0.000285;0.070403;,
  0.997425;0.000565;-0.071714;,
  -0.010705;0.018260;-0.999776;,
  -0.147834;0.018084;-0.988847;,
  -0.996943;0.000142;0.078126;,
  -0.853003;-0.000576;0.521906;,
  0.010693;-0.000000;0.999943;,
  0.851473;-0.000808;0.524398;,
  0.941449;-0.214824;0.259854;,
  0.939098;-0.155717;-0.306346;,
  -0.010644;0.108625;-0.994026;,
  -0.333050;0.122387;-0.934933;,
  -0.944444;-0.209506;0.253245;,
  -0.855071;-0.000817;0.518511;,
  0.010692;0.000000;0.999943;,
  0.729657;-0.150099;0.667136;,
  0.928121;-0.371241;0.027772;,
  0.349924;0.171482;-0.920949;,
  -0.010526;0.183679;-0.982930;,
  -0.423802;0.167065;-0.890214;,
  -0.915633;-0.393990;0.079928;,
  -0.756473;-0.139315;0.639015;,
  0.010693;0.000000;0.999943;,
  -0.001297;-0.999989;-0.004459;,
  0.000000;1.000000;0.000000;,
  -0.397978;-0.002313;0.917392;,
  -0.396741;-0.000788;0.917930;,
  -0.393086;-0.000554;0.919501;,
  -0.387064;0.000657;0.922053;,
  -0.383455;0.003153;0.923554;,
  0.416994;-0.002308;0.908906;,
  0.415772;-0.000786;0.909469;,
  0.412162;-0.000554;0.911110;,
  0.406210;0.000657;0.913779;,
  0.402642;0.003150;0.915352;,
  -0.000060;0.999989;-0.004748;,
  0.001548;0.999996;-0.002592;,
  -0.016422;0.994618;-0.102303;,
  -0.054820;0.979536;-0.193660;,
  -0.001244;0.969375;-0.245583;,
  0.045905;0.977182;-0.207384;,
  0.006347;0.993007;-0.117882;,
  -0.001588;0.999995;-0.002739;,
  0.834185;-0.096829;-0.542918;,
  0.674872;-0.417637;0.608381;,
  -0.889828;-0.137068;-0.435223;,
  -0.712735;-0.400980;0.575521;,
  0.126642;-0.008504;-0.991912;,
  0.126647;-0.002472;-0.991945;,
  -0.999117;-0.022105;-0.035740;,
  -0.999526;-0.011026;-0.028750;,
  0.126626;0.018091;-0.991786;,
  -0.999753;0.000283;-0.022213;,
  0.125911;0.107631;-0.986185;,
  -0.963556;-0.265806;-0.030126;,
  0.305040;-0.952083;0.022115;,
  0.238255;-0.940171;0.243544;,
  0.188975;-0.947981;-0.256164;,
  -0.005525;-0.880752;-0.473545;,
  -0.196980;-0.947385;-0.252310;,
  -0.308626;-0.950971;0.020096;,
  -0.235659;-0.941673;0.240242;,
  0.004483;-0.907860;0.419250;,
  -0.154172;-0.896461;0.415438;,
  0.163025;-0.896275;0.412448;,
  -0.003910;0.990907;-0.134492;,
  -0.001333;0.999998;-0.001512;,
  -0.001746;0.999997;-0.001496;,
  -0.005793;0.964305;-0.264730;,
  0.028828;0.973649;-0.226224;,
  -0.027967;0.972766;-0.230094;,
  -0.000188;0.990904;-0.134570;,
  -0.002675;0.963727;-0.266877;,
  0.001398;0.999999;-0.000712;,
  -0.000052;0.999990;-0.004484;,
  0.001219;0.999994;-0.003229;,
  -0.001295;0.999994;-0.003209;,
  -0.775589;0.065852;0.627794;,
  0.006667;-0.016489;0.999842;,
  -0.990725;0.134691;0.017953;,
  -0.781963;0.094805;-0.616073;,
  -0.010816;0.039042;-0.999179;,
  0.738774;0.110866;-0.664772;,
  0.986703;0.161829;0.015098;,
  0.745656;0.079189;0.661609;;
  92;
  4;0,1,9,8;,
  4;1,2,10,9;,
  4;64,3,11,65;,
  4;3,4,12,11;,
  4;66,5,13,67;,
  4;5,6,14,13;,
  4;45,46,7,15;,
  4;50,51,0,8;,
  4;8,9,17,16;,
  4;9,10,18,17;,
  4;65,11,19,68;,
  4;11,12,20,19;,
  4;67,13,21,69;,
  4;13,14,22,21;,
  4;44,45,15,23;,
  4;49,50,8,16;,
  4;16,17,25,24;,
  4;17,18,26,25;,
  4;68,19,27,70;,
  4;19,20,28,27;,
  4;69,21,29,71;,
  4;21,22,30,29;,
  4;43,44,23,31;,
  4;48,49,16,24;,
  3;33,61,25;,
  3;61,32,25;,
  3;32,24,25;,
  3;34,60,26;,
  3;60,33,26;,
  3;33,25,26;,
  4;70,27,35,34;,
  4;27,28,36,35;,
  3;29,37,71;,
  3;37,62,71;,
  3;62,36,28;,
  3;30,38,29;,
  3;38,63,29;,
  3;63,37,29;,
  4;42,43,31,39;,
  4;47,48,24,32;,
  3;72,73,40;,
  3;74,72,40;,
  3;75,74,40;,
  3;76,75,40;,
  3;77,76,40;,
  3;78,77,40;,
  4;79,80,78,40;,
  4;73,81,79,40;,
  3;82,58,83;,
  3;83,58,84;,
  3;58,59,84;,
  3;82,85,58;,
  3;85,86,58;,
  3;86,57,58;,
  4;86,56,56,57;,
  4;56,87,55,56;,
  3;55,87,54;,
  3;54,87,88;,
  3;87,89,88;,
  3;53,54,90;,
  3;54,88,90;,
  3;88,41,90;,
  3;52,53,91;,
  3;91,53,92;,
  3;53,90,92;,
  3;84,59,93;,
  3;93,59,91;,
  3;59,52,91;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;,
  4;30,43,42,38;,
  4;22,44,43,30;,
  4;14,45,44,22;,
  4;6,46,45,14;,
  4;31,48,47,39;,
  4;23,49,48,31;,
  4;15,50,49,23;,
  4;7,51,50,15;,
  4;94,95,95,94;,
  4;96,94,94,96;,
  4;97,96,96,97;,
  4;98,97,97,98;,
  4;99,98,98,99;,
  4;100,99,99,100;,
  4;101,100,100,101;,
  4;95,101,101,95;;
 }
 MeshTextureCoords {
  101;
  0.633134;0.409111;,
  0.641455;0.409111;,
  0.641455;0.384085;,
  0.633134;0.384085;,
  0.649776;0.409111;,
  0.649776;0.384085;,
  0.658097;0.409111;,
  0.658097;0.384085;,
  0.666418;0.409111;,
  0.666418;0.384085;,
  0.674739;0.409111;,
  0.674739;0.384085;,
  0.683060;0.409111;,
  0.683060;0.384085;,
  0.685063;0.384085;,
  0.685250;0.409111;,
  0.691381;0.409111;,
  0.691381;0.384085;,
  0.697700;0.384085;,
  0.697513;0.409111;,
  0.699702;0.409111;,
  0.699702;0.384085;,
  0.641455;0.359059;,
  0.633134;0.359059;,
  0.649776;0.359059;,
  0.658097;0.359059;,
  0.666418;0.359059;,
  0.674739;0.359059;,
  0.683060;0.359059;,
  0.684940;0.359059;,
  0.691381;0.359059;,
  0.697823;0.359059;,
  0.699702;0.359059;,
  0.641455;0.334034;,
  0.633134;0.334034;,
  0.649776;0.334034;,
  0.658097;0.334034;,
  0.666418;0.334034;,
  0.674739;0.334034;,
  0.683060;0.334034;,
  0.684975;0.334034;,
  0.691381;0.334034;,
  0.697788;0.334034;,
  0.699702;0.334034;,
  0.641455;0.309008;,
  0.636065;0.309008;,
  0.633134;0.309008;,
  0.649776;0.309008;,
  0.646839;0.309008;,
  0.658097;0.309008;,
  0.666418;0.309008;,
  0.674739;0.309008;,
  0.670575;0.309008;,
  0.683060;0.309008;,
  0.678827;0.309008;,
  0.684899;0.309008;,
  0.691381;0.309008;,
  0.697864;0.309008;,
  0.699702;0.309008;,
  0.666418;0.492321;,
  0.695838;0.480135;,
  0.666418;0.452380;,
  0.636999;0.480135;,
  0.624813;0.450716;,
  0.636999;0.421296;,
  0.666418;0.409111;,
  0.695838;0.421296;,
  0.708023;0.450716;,
  0.699044;0.429037;,
  0.699044;0.472394;,
  0.666418;0.226048;,
  0.666418;0.243415;,
  0.685478;0.233943;,
  0.695838;0.238234;,
  0.680256;0.254698;,
  0.647384;0.233932;,
  0.636999;0.238234;,
  0.649852;0.251814;,
  0.624813;0.267653;,
  0.634685;0.268048;,
  0.636999;0.297073;,
  0.651210;0.283666;,
  0.666418;0.294346;,
  0.666418;0.309258;,
  0.651695;0.303160;,
  0.681077;0.283147;,
  0.695838;0.297073;,
  0.680869;0.303273;,
  0.693108;0.268250;,
  0.708023;0.267653;,
  0.698530;0.290573;,
  0.698530;0.244734;,
  0.677183;0.279473;,
  0.686406;0.268518;,
  0.666418;0.269318;,
  0.666418;0.288144;,
  0.654830;0.280251;,
  0.643350;0.268395;,
  0.654222;0.256431;,
  0.666418;0.250727;,
  0.675671;0.259541;;
 }
}

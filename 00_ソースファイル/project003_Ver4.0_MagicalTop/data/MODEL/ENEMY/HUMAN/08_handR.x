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
 136;
 2.88060;-2.81930;-2.76990;,
 2.48099;-1.31723;-2.76990;,
 3.82241;-1.00526;-2.83083;,
 4.15686;-2.62125;-2.83083;,
 4.45574;-2.67876;-0.52002;,
 4.12127;-1.00526;-0.52002;,
 2.77985;-1.31723;-0.52002;,
 3.11430;-2.86956;-0.52002;,
 4.37360;-2.67876;-1.67543;,
 3.03218;-2.86956;-1.67543;,
 2.88060;-2.81930;-2.76990;,
 4.15686;-2.62125;-2.83083;,
 3.11430;-2.86956;-0.52002;,
 4.45574;-2.67876;-0.52002;,
 2.69774;-1.31723;-1.67543;,
 3.03218;-2.86956;-1.67543;,
 2.77985;-1.31723;-0.52002;,
 4.03916;-1.00526;-1.67543;,
 4.37360;-2.67876;-1.67543;,
 4.15686;-2.62125;-2.83083;,
 3.82241;-1.00526;-2.83083;,
 4.12127;-1.00526;-0.52002;,
 4.03916;-1.00526;-1.67543;,
 2.69774;-1.31723;-1.67543;,
 2.48099;-1.31723;-2.76990;,
 3.82241;-1.00526;-2.83083;,
 1.80765;-1.50495;-1.49387;,
 1.62140;-2.54084;-1.49387;,
 2.48099;-1.31723;-2.76990;,
 1.61628;-1.37372;-2.53766;,
 2.88060;-2.81930;-2.76990;,
 1.49519;-2.47869;-2.38149;,
 0.52011;-0.94499;-1.43016;,
 0.47510;-0.61515;-1.29911;,
 0.47510;-0.61515;-1.29911;,
 0.52011;-0.94499;-1.43016;,
 0.52011;-0.94499;-1.43016;,
 0.47510;-0.61515;-1.29911;,
 4.76465;1.62078;-0.00053;,
 3.18029;2.07336;-0.00054;,
 3.00452;2.06688;0.71141;,
 4.64372;1.67390;1.11546;,
 2.58415;2.06811;0.96188;,
 4.38329;1.67895;1.91336;,
 2.07986;2.07659;0.94317;,
 1.81602;1.82909;1.55931;,
 1.68417;2.08902;0.66242;,
 1.06241;1.94975;1.02623;,
 1.54830;2.10081;-0.00048;,
 0.80393;1.97205;-0.00044;,
 5.13987;1.26450;-0.00051;,
 5.01161;1.32204;1.14506;,
 4.86941;1.31862;2.16833;,
 1.60016;1.54127;2.04708;,
 0.56432;1.57394;1.31205;,
 0.20855;1.60461;-0.00042;,
 5.48634;0.55293;-0.00051;,
 5.40519;0.49877;1.17461;,
 5.29039;0.38637;2.20947;,
 1.46033;0.58640;2.35235;,
 0.24258;0.62480;1.48827;,
 -0.17567;0.66089;-0.00042;,
 5.79053;-0.39870;-0.00042;,
 5.72757;-0.46725;1.17468;,
 5.58767;-0.57834;2.23427;,
 3.10716;-1.23681;2.09397;,
 1.40514;-0.46841;2.44985;,
 0.12476;-0.42804;1.54131;,
 -0.31500;-0.39006;-0.00044;,
 5.05851;-2.00783;-0.00048;,
 5.00874;-2.02682;1.10697;,
 4.90478;-2.07291;2.20023;,
 1.44009;-1.51988;2.33004;,
 0.22235;-1.48148;1.46597;,
 -0.19589;-1.44538;-0.00053;,
 3.60390;-2.31242;-0.00053;,
 3.54117;-2.32577;1.16372;,
 3.36078;-2.25659;2.12861;,
 0.35750;-1.79630;1.27969;,
 0.01875;-1.86587;-0.00050;,
 3.25122;-1.03652;-0.00047;,
 3.19079;-0.91052;1.17092;,
 3.10716;-1.23681;2.09397;,
 3.49368;-1.10154;1.37633;,
 3.10716;-1.23681;2.09397;,
 1.44009;-1.51988;2.33004;,
 0.35750;-1.79630;1.27969;,
 3.52665;-1.21989;0.70877;,
 0.01875;-1.86587;-0.00050;,
 3.39075;-1.32486;-0.00044;,
 2.36723;2.12813;-0.00044;,
 2.36723;2.12813;-0.00044;,
 2.36723;2.12813;-0.00044;,
 2.36723;2.12813;-0.00044;,
 2.36723;2.12813;-0.00044;,
 1.44009;-1.51988;2.33004;,
 3.10716;-1.23681;2.09397;,
 3.10716;-1.23681;2.09397;,
 4.64372;1.67390;-1.11642;,
 3.00452;2.06688;-0.71235;,
 4.38329;1.67895;-1.91430;,
 2.58415;2.06811;-0.96284;,
 1.81602;1.82909;-1.56027;,
 2.07986;2.07659;-0.94413;,
 1.06241;1.94975;-1.02719;,
 1.68417;2.08902;-0.66336;,
 5.01161;1.32204;-1.14602;,
 4.86941;1.31862;-2.16927;,
 1.60016;1.54127;-2.04803;,
 0.56432;1.57394;-1.31301;,
 5.40519;0.49877;-1.17557;,
 5.29039;0.38637;-2.21042;,
 1.46033;0.58640;-2.35331;,
 0.24258;0.62480;-1.48923;,
 5.72757;-0.46725;-1.17564;,
 5.58767;-0.57834;-2.23523;,
 3.10716;-1.23681;-2.09493;,
 1.40514;-0.46841;-2.45080;,
 0.12476;-0.42804;-1.54227;,
 5.00874;-2.02682;-1.10792;,
 4.90478;-2.07291;-2.20119;,
 1.61628;-1.37372;-2.53766;,
 0.22235;-1.48148;-1.46693;,
 3.54117;-2.32577;-1.16466;,
 3.36078;-2.25659;-2.12955;,
 0.35750;-1.79630;-1.28064;,
 3.19079;-0.91052;-1.17188;,
 3.10716;-1.23681;-2.09493;,
 3.10716;-1.23681;-2.09493;,
 3.49368;-1.10154;-1.37729;,
 1.61628;-1.37372;-2.53766;,
 3.52665;-1.21989;-0.70973;,
 0.35750;-1.79630;-1.28064;,
 1.61628;-1.37372;-2.53766;,
 3.10716;-1.23681;-2.09493;,
 3.10716;-1.23681;-2.09493;;
 
 103;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,9,8,13;,
 4;14,15,12,16;,
 4;17,18,19,20;,
 4;13,18,17,21;,
 4;22,23,16,21;,
 4;24,23,22,25;,
 4;14,26,27,15;,
 4;28,29,26,14;,
 4;30,31,29,28;,
 4;15,27,31,30;,
 4;29,32,33,26;,
 4;27,34,35,31;,
 3;31,36,29;,
 3;26,37,27;,
 4;38,39,40,41;,
 4;41,40,42,43;,
 4;43,42,44,45;,
 4;45,44,46,47;,
 4;47,46,48,49;,
 4;50,38,41,51;,
 4;51,41,43,52;,
 4;52,43,45,53;,
 4;53,45,47,54;,
 4;54,47,49,55;,
 4;56,50,51,57;,
 4;57,51,52,58;,
 4;58,52,53,59;,
 4;59,53,54,60;,
 4;60,54,55,61;,
 4;62,56,57,63;,
 4;63,57,58,64;,
 3;65,64,58;,
 3;65,58,59;,
 3;65,59,66;,
 4;66,59,60,67;,
 4;67,60,61,68;,
 4;69,62,63,70;,
 4;70,63,64,71;,
 4;72,66,67,73;,
 4;73,67,68,74;,
 4;75,69,70,76;,
 4;76,70,71,77;,
 4;78,73,74,79;,
 4;80,75,76,81;,
 4;81,76,77,82;,
 3;83,84,85;,
 4;83,85,86,87;,
 4;87,86,88,89;,
 3;90,40,39;,
 3;91,42,40;,
 3;92,44,42;,
 3;93,46,44;,
 3;94,48,46;,
 3;78,95,73;,
 3;71,96,77;,
 3;64,97,71;,
 3;72,97,66;,
 4;39,38,98,99;,
 4;99,98,100,101;,
 4;101,100,102,103;,
 4;103,102,104,105;,
 4;105,104,49,48;,
 4;38,50,106,98;,
 4;98,106,107,100;,
 4;100,107,108,102;,
 4;102,108,109,104;,
 4;104,109,55,49;,
 4;50,56,110,106;,
 4;106,110,111,107;,
 4;107,111,112,108;,
 4;108,112,113,109;,
 4;109,113,61,55;,
 4;56,62,114,110;,
 4;110,114,115,111;,
 3;115,116,111;,
 3;111,116,112;,
 3;112,116,117;,
 4;112,117,118,113;,
 4;113,118,68,61;,
 4;62,69,119,114;,
 4;114,119,120,115;,
 4;117,121,122,118;,
 4;118,122,74,68;,
 4;69,75,123,119;,
 4;119,123,124,120;,
 4;122,125,79,74;,
 4;75,80,126,123;,
 4;123,126,127,124;,
 3;128,129,130;,
 4;130,129,131,132;,
 4;132,131,89,88;,
 3;99,90,39;,
 3;101,91,99;,
 3;103,92,101;,
 3;105,93,103;,
 3;48,94,105;,
 3;133,125,122;,
 3;134,120,124;,
 3;135,115,120;,
 3;135,121,117;;
 
 MeshMaterialList {
  1;
  103;
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
   0.116078;0.160000;0.018824;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  119;
  -0.157872;-0.051783;-0.986101;,
  -0.044469;-0.010466;-0.998956;,
  0.000000;0.000000;1.000000;,
  0.016809;-0.999238;-0.035229;,
  0.143060;-0.988889;-0.040411;,
  -0.975218;-0.210109;0.069311;,
  0.972830;0.196136;-0.123017;,
  -0.509302;-0.058444;-0.858601;,
  0.143274;-0.006829;0.989659;,
  0.315740;0.786391;-0.530940;,
  0.157605;0.987502;-0.000002;,
  0.129312;0.989939;0.057437;,
  0.055090;0.986400;0.154854;,
  -0.026473;0.980094;0.196759;,
  -0.083234;0.990644;0.108151;,
  -0.104774;0.994496;0.000000;,
  0.483489;0.875351;-0.000002;,
  0.445241;0.891149;0.087259;,
  0.208872;0.912505;0.351721;,
  -0.093597;0.882805;0.460320;,
  -0.273920;0.928346;0.251280;,
  -0.359852;0.933009;0.000004;,
  0.807464;0.589916;-0.000002;,
  0.792515;0.601058;0.103199;,
  0.573649;0.734748;0.362038;,
  -0.213144;0.573381;0.791078;,
  -0.607811;0.650472;0.455470;,
  -0.765894;0.642967;0.000009;,
  0.927449;0.373951;-0.000001;,
  0.923157;0.368509;0.109465;,
  0.920702;0.363810;0.141247;,
  -0.179074;0.125411;0.975810;,
  -0.789268;0.243499;0.563707;,
  -0.967675;0.252201;0.000010;,
  0.998340;-0.057592;0.000001;,
  0.994478;-0.059355;0.086548;,
  0.991193;-0.061150;0.117464;,
  -0.207147;-0.009804;0.978261;,
  -0.816502;0.002486;0.577337;,
  -0.999967;0.008155;0.000004;,
  0.622625;-0.782521;0.000000;,
  0.612495;-0.789598;0.037220;,
  0.601885;-0.796278;0.060627;,
  -0.773634;-0.344870;0.531558;,
  -0.952877;-0.303356;-0.000003;,
  0.202270;-0.979330;0.000000;,
  0.179450;-0.983645;0.015467;,
  0.156468;-0.987065;0.034948;,
  -0.967380;-0.253331;0.000001;,
  -0.963564;-0.246905;-0.102866;,
  0.043867;-0.018379;0.998868;,
  -0.330384;-0.296733;0.895989;,
  -0.693593;-0.551391;0.463570;,
  -0.876429;-0.481531;-0.000004;,
  0.183168;-0.983078;0.002484;,
  0.194848;-0.977250;0.083764;,
  0.177992;-0.984032;0.000003;,
  0.015564;0.999879;0.000001;,
  0.129310;0.989939;-0.057438;,
  0.055091;0.986401;-0.154851;,
  -0.026473;0.980095;-0.196755;,
  -0.083235;0.990644;-0.108149;,
  0.445241;0.891148;-0.087261;,
  0.208872;0.912504;-0.351723;,
  -0.093597;0.882804;-0.460322;,
  -0.273924;0.928346;-0.251277;,
  0.792515;0.601057;-0.103202;,
  0.573649;0.734747;-0.362041;,
  -0.213142;0.573381;-0.791078;,
  -0.607814;0.650473;-0.455464;,
  0.923157;0.368507;-0.109466;,
  0.920702;0.363810;-0.141248;,
  -0.179071;0.125415;-0.975810;,
  -0.789272;0.243502;-0.563699;,
  0.994478;-0.059357;-0.086548;,
  0.991193;-0.061150;-0.117465;,
  -0.182908;0.068789;-0.980720;,
  -0.818606;0.008664;-0.574290;,
  0.612495;-0.789598;-0.037219;,
  0.601885;-0.796278;-0.060627;,
  -0.774421;-0.339296;-0.533995;,
  0.179450;-0.983645;-0.015467;,
  0.156467;-0.987065;-0.034948;,
  -0.963564;-0.246905;0.102867;,
  0.062813;0.025532;-0.997699;,
  0.107193;0.964113;-0.242891;,
  -0.686933;-0.553111;-0.471371;,
  0.127836;-0.991764;0.007850;,
  0.193846;-0.975548;-0.103585;,
  -0.046478;-0.997772;-0.047860;,
  0.145175;-0.986871;-0.070775;,
  0.140812;-0.989986;-0.010010;,
  0.964612;0.196202;-0.176147;,
  0.978235;0.195503;-0.069532;,
  -0.225942;0.973686;0.029774;,
  -0.190370;0.980549;0.047778;,
  -0.226492;0.973880;0.016096;,
  -0.172110;0.983022;0.063603;,
  -0.225350;0.973309;0.043447;,
  0.157124;0.009604;0.987532;,
  -0.394416;-0.142084;-0.907881;,
  -0.524735;-0.815922;0.242744;,
  -0.719668;-0.515218;0.465434;,
  -0.714543;-0.020523;-0.699291;,
  0.129357;-0.023259;0.991325;,
  -0.004774;0.027190;0.999619;,
  0.013117;0.185847;0.982491;,
  -0.057939;0.005561;0.998305;,
  -0.956601;-0.239679;-0.165735;,
  0.152277;-0.982929;-0.103258;,
  -0.053154;0.012620;0.998507;,
  -0.048910;0.021751;0.998566;,
  0.013119;0.185855;-0.982490;,
  -0.004773;0.027198;-0.999619;,
  -0.057941;0.005569;-0.998304;,
  -0.956602;-0.239677;0.165735;,
  0.042071;-0.985706;0.163138;,
  -0.053157;0.012612;-0.998507;,
  -0.048918;0.021734;-0.998566;;
  103;
  4;0,0,1,1;,
  4;2,2,2,2;,
  4;4,3,89,90;,
  4;91,3,4,91;,
  4;5,5,5,5;,
  4;6,6,92,92;,
  4;93,6,6,93;,
  4;94,95,96,96;,
  4;97,95,94,98;,
  4;99,8,8,99;,
  4;97,85,85,95;,
  4;0,7,100,0;,
  4;3,101,101,89;,
  4;85,9,9,85;,
  4;101,102,102,101;,
  3;7,103,100;,
  3;8,104,8;,
  4;16,10,11,17;,
  4;17,11,12,18;,
  4;18,12,13,19;,
  4;19,13,14,20;,
  4;20,14,15,21;,
  4;22,16,17,23;,
  4;23,17,18,24;,
  4;24,18,19,25;,
  4;25,19,20,26;,
  4;26,20,21,27;,
  4;28,22,23,29;,
  4;29,23,24,30;,
  4;105,106,25,31;,
  4;31,25,26,32;,
  4;32,26,27,33;,
  4;34,28,29,35;,
  4;35,29,30,36;,
  3;50,107,105;,
  3;50,105,31;,
  3;50,31,37;,
  4;37,31,32,38;,
  4;38,32,33,39;,
  4;40,34,35,41;,
  4;41,35,36,42;,
  4;51,37,38,43;,
  4;43,38,39,44;,
  4;45,40,41,46;,
  4;46,41,42,47;,
  4;52,43,44,53;,
  4;48,48,49,49;,
  4;49,49,108,108;,
  3;54,109,54;,
  4;54,54,55,55;,
  4;55,55,56,56;,
  3;57,11,10;,
  3;57,12,11;,
  3;57,13,12;,
  3;57,14,13;,
  3;57,15,14;,
  3;52,51,43;,
  3;110,50,111;,
  3;107,50,110;,
  3;51,50,37;,
  4;10,16,62,58;,
  4;58,62,63,59;,
  4;59,63,64,60;,
  4;60,64,65,61;,
  4;61,65,21,15;,
  4;16,22,66,62;,
  4;62,66,67,63;,
  4;63,67,68,64;,
  4;64,68,69,65;,
  4;65,69,27,21;,
  4;22,28,70,66;,
  4;66,70,71,67;,
  4;112,113,72,68;,
  4;68,72,73,69;,
  4;69,73,33,27;,
  4;28,34,74,70;,
  4;70,74,75,71;,
  3;114,84,113;,
  3;113,84,72;,
  3;72,84,76;,
  4;72,76,77,73;,
  4;73,77,39,33;,
  4;34,40,78,74;,
  4;74,78,79,75;,
  4;76,100,80,77;,
  4;77,80,44,39;,
  4;40,45,81,78;,
  4;78,81,82,79;,
  4;80,86,53,44;,
  4;48,48,83,83;,
  4;83,83,115,115;,
  3;116,87,87;,
  4;87,87,88,88;,
  4;88,88,56,56;,
  3;58,57,10;,
  3;59,57,58;,
  3;60,57,59;,
  3;61,57,60;,
  3;15,57,61;,
  3;100,86,80;,
  3;84,117,118;,
  3;84,114,117;,
  3;84,100,76;;
 }
 MeshTextureCoords {
  136;
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  0.500000;0.500000;,
  0.607480;0.722190;,
  0.607480;0.735480;,
  0.620770;0.735480;,
  0.620770;0.722190;,
  0.634060;0.735480;,
  0.634060;0.722190;,
  0.647360;0.735480;,
  0.647360;0.722190;,
  0.660650;0.735480;,
  0.660650;0.722190;,
  0.673940;0.735480;,
  0.673940;0.722190;,
  0.607480;0.708890;,
  0.620770;0.708890;,
  0.634060;0.708890;,
  0.647360;0.708890;,
  0.660650;0.708890;,
  0.673940;0.708890;,
  0.607480;0.695600;,
  0.620770;0.695600;,
  0.634060;0.695600;,
  0.647360;0.695600;,
  0.660650;0.695600;,
  0.673940;0.695600;,
  0.607480;0.682310;,
  0.620770;0.682310;,
  0.634060;0.682310;,
  0.640980;0.682310;,
  0.647360;0.682310;,
  0.660650;0.682310;,
  0.673940;0.682310;,
  0.607480;0.669020;,
  0.620770;0.669020;,
  0.634060;0.669020;,
  0.647360;0.669020;,
  0.660650;0.669020;,
  0.673940;0.669020;,
  0.607480;0.655730;,
  0.620770;0.655730;,
  0.634060;0.655730;,
  0.660650;0.655730;,
  0.673940;0.655730;,
  0.607480;0.642430;,
  0.620770;0.642430;,
  0.634060;0.642430;,
  0.647360;0.629140;,
  0.638490;0.642430;,
  0.647360;0.642430;,
  0.660650;0.642430;,
  0.660650;0.629140;,
  0.673940;0.642430;,
  0.673940;0.629140;,
  0.614120;0.748770;,
  0.627420;0.748770;,
  0.640710;0.748770;,
  0.654000;0.748770;,
  0.667290;0.748770;,
  0.647360;0.662370;,
  0.640680;0.662370;,
  0.641140;0.675660;,
  0.620770;0.722190;,
  0.620770;0.735480;,
  0.634060;0.722190;,
  0.634060;0.735480;,
  0.647360;0.722190;,
  0.647360;0.735480;,
  0.660650;0.722190;,
  0.660650;0.735480;,
  0.620770;0.708890;,
  0.634060;0.708890;,
  0.647360;0.708890;,
  0.660650;0.708890;,
  0.620770;0.695600;,
  0.634060;0.695600;,
  0.647360;0.695600;,
  0.660650;0.695600;,
  0.620770;0.682310;,
  0.634060;0.682310;,
  0.640980;0.682310;,
  0.647360;0.682310;,
  0.660650;0.682310;,
  0.620770;0.669020;,
  0.634060;0.669020;,
  0.647360;0.669020;,
  0.660650;0.669020;,
  0.620770;0.655730;,
  0.634060;0.655730;,
  0.660650;0.655730;,
  0.620770;0.642430;,
  0.634060;0.642430;,
  0.638490;0.642430;,
  0.647360;0.629140;,
  0.647360;0.642430;,
  0.660650;0.629140;,
  0.660650;0.642430;,
  0.647360;0.662370;,
  0.640680;0.662370;,
  0.641140;0.675660;;
 }
}

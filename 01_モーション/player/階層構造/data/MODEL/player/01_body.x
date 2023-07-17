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
 170;
 0.00000;-4.25885;4.41648;,
 0.00000;-6.53157;-0.02411;,
 5.63641;-4.25885;3.11586;,
 4.55176;21.76709;2.81531;,
 0.00000;17.26658;-0.02411;,
 0.00000;21.48126;3.99244;,
 7.55843;10.89826;5.02662;,
 7.22954;16.43043;5.30125;,
 0.00000;16.14542;7.38338;,
 0.00000;10.89826;7.11871;,
 6.62531;19.02494;4.77293;,
 0.00000;18.46651;6.75994;,
 0.00000;3.08256;6.79597;,
 7.56122;3.08256;4.79840;,
 7.57109;-1.51976;4.80515;,
 0.00000;-1.51976;6.80552;,
 6.20004;20.67307;3.91614;,
 0.00000;20.38723;5.54927;,
 4.44697;18.75235;-5.54841;,
 4.81762;16.43043;-6.20218;,
 0.00000;16.43043;-7.55531;,
 0.00000;18.34222;-6.80816;,
 6.62531;19.02494;-4.75457;,
 6.86925;16.43043;-5.34947;,
 3.85899;20.25358;-4.72588;,
 6.20004;20.51235;-3.96925;,
 0.00000;20.00229;-5.92654;,
 5.21075;10.00066;-5.88357;,
 0.00000;9.85901;-7.16693;,
 7.19814;10.14696;-5.07485;,
 2.84788;21.15977;-3.87985;,
 4.62409;21.52597;-2.91213;,
 0.00000;20.81253;-4.92294;,
 5.37102;3.08256;-5.61881;,
 0.00000;3.08256;-6.84420;,
 7.44161;3.08256;-4.84663;,
 5.37705;-1.51976;-5.62665;,
 3.86316;-4.25885;-3.66685;,
 0.00000;-4.25885;-4.46470;,
 0.00000;-1.51976;-6.85374;,
 7.57109;-1.51976;-4.85337;,
 5.63641;-4.25885;-3.16408;,
 5.30981;0.78190;-5.53959;,
 0.00000;0.78190;-6.74761;,
 7.46145;0.78190;-4.77833;,
 7.46145;0.78190;4.73010;,
 0.00000;0.78190;6.69938;,
 5.92632;21.64654;-1.46935;,
 7.38941;20.49059;-1.99791;,
 6.70394;21.76709;-0.02655;,
 0.00000;17.26658;-0.02411;,
 9.24105;19.80362;-2.38934;,
 8.73355;16.43043;-2.68680;,
 8.85613;10.32181;-2.54948;,
 9.27020;16.43043;-0.02411;,
 9.43786;10.41318;-0.02411;,
 9.01546;3.08256;-2.43537;,
 9.92373;3.08256;-0.02411;,
 9.26372;0.78190;-2.40123;,
 10.26679;0.78190;-0.02411;,
 9.51612;-1.51976;-2.43874;,
 10.69604;-1.51976;-0.02411;,
 6.58645;-4.25885;-1.59409;,
 7.00325;-4.25885;-0.02411;,
 0.00000;-6.53157;-0.02411;,
 7.38941;20.67307;1.94479;,
 5.89015;21.76709;1.39438;,
 9.24105;19.80362;2.37441;,
 8.73355;16.43043;2.63857;,
 8.85613;10.65573;2.50125;,
 9.01546;3.08256;2.38714;,
 9.26372;0.78190;2.35300;,
 9.51612;-1.51976;2.39052;,
 6.58645;-4.25885;1.54587;,
 9.96088;20.26075;-0.02411;,
 7.61347;21.16536;-0.02655;,
 9.96088;20.26075;-0.02411;,
 6.70394;21.76709;-0.02655;,
 0.00000;21.82741;-3.13179;,
 0.00000;22.77797;-0.02655;,
 3.18246;22.07801;-1.98558;,
 0.00000;22.77797;-0.02655;,
 4.44105;22.22534;-0.02655;,
 0.00000;22.77797;-0.02655;,
 3.18246;22.22534;1.93247;,
 0.00000;22.77797;-0.02655;,
 0.00000;22.22534;2.74392;,
 0.00000;20.24605;-5.20132;,
 5.05100;21.01281;-3.19030;,
 6.55030;21.01281;-0.02655;,
 5.05100;21.01281;3.13719;,
 0.00000;21.01281;4.44765;,
 0.00000;21.81229;-2.36304;,
 0.00000;25.37747;-1.63656;,
 1.75614;25.37747;-1.12732;,
 2.78333;21.81229;-1.64102;,
 2.45689;25.37747;0.10212;,
 3.36909;21.81229;0.10212;,
 1.75614;25.37747;1.33155;,
 2.78333;21.81229;1.84525;,
 0.00000;25.37747;1.84081;,
 0.00000;21.81229;2.56729;,
 -5.63641;-4.25885;3.11586;,
 -4.55176;21.76709;2.81531;,
 -7.22954;16.43043;5.30125;,
 -7.55843;10.89826;5.02662;,
 -6.62531;19.02494;4.77293;,
 -7.56122;3.08256;4.79840;,
 -7.57109;-1.51976;4.80515;,
 -6.20004;20.67307;3.91614;,
 -4.81762;16.43043;-6.20218;,
 -4.44697;18.75235;-5.54841;,
 -6.86925;16.43043;-5.34947;,
 -6.62531;19.02494;-4.75457;,
 -6.20004;20.51235;-3.96925;,
 -3.85899;20.25358;-4.72588;,
 -5.21075;10.00066;-5.88357;,
 -7.19814;10.14696;-5.07485;,
 -4.62409;21.52597;-2.91213;,
 -2.84788;21.15977;-3.87985;,
 -5.37102;3.08256;-5.61881;,
 -7.44161;3.08256;-4.84663;,
 -3.86316;-4.25885;-3.66685;,
 -5.37705;-1.51976;-5.62665;,
 -5.63641;-4.25885;-3.16408;,
 -7.57109;-1.51976;-4.85337;,
 -5.30981;0.78190;-5.53959;,
 -7.46145;0.78190;-4.77833;,
 -7.46145;0.78190;4.73010;,
 -7.38941;20.49059;-1.99791;,
 -5.92632;21.64654;-1.46935;,
 -6.70394;21.76709;-0.02655;,
 -9.24105;19.80362;-2.38934;,
 -8.73355;16.43043;-2.68680;,
 -9.43786;10.41318;-0.02411;,
 -9.27020;16.43043;-0.02411;,
 -8.85613;10.32181;-2.54948;,
 -9.92373;3.08256;-0.02411;,
 -9.01546;3.08256;-2.43537;,
 -10.26679;0.78190;-0.02411;,
 -9.26372;0.78190;-2.40123;,
 -10.69604;-1.51976;-0.02411;,
 -9.51612;-1.51976;-2.43874;,
 -7.00325;-4.25885;-0.02411;,
 -6.58645;-4.25885;-1.59409;,
 -5.89015;21.76709;1.39438;,
 -7.38941;20.67307;1.94479;,
 -9.24105;19.80362;2.37441;,
 -8.73355;16.43043;2.63857;,
 -8.85613;10.65573;2.50125;,
 -9.01546;3.08256;2.38714;,
 -9.26372;0.78190;2.35300;,
 -9.51612;-1.51976;2.39052;,
 -6.58645;-4.25885;1.54587;,
 -9.96088;20.26075;-0.02411;,
 -9.96088;20.26075;-0.02411;,
 -7.61347;21.16536;-0.02655;,
 -6.70394;21.76709;-0.02655;,
 -3.18246;22.07801;-1.98558;,
 -4.44105;22.22534;-0.02655;,
 -3.18246;22.22534;1.93247;,
 -5.05100;21.01281;-3.19030;,
 -6.55030;21.01281;-0.02655;,
 -5.05100;21.01281;3.13719;,
 -2.78333;21.81229;-1.64102;,
 -1.75614;25.37747;-1.12732;,
 -3.36909;21.81229;0.10212;,
 -2.45689;25.37747;0.10212;,
 -2.78333;21.81229;1.84525;,
 -1.75614;25.37747;1.33155;;
 
 176;
 3;0,1,2;,
 3;3,4,5;,
 4;6,7,8,9;,
 4;7,10,11,8;,
 4;12,13,6,9;,
 4;14,15,0,2;,
 4;16,17,11,10;,
 4;16,3,5,17;,
 4;18,19,20,21;,
 4;18,22,23,19;,
 4;18,24,25,22;,
 4;18,21,26,24;,
 4;19,27,28,20;,
 4;19,23,29,27;,
 4;24,30,31,25;,
 4;24,26,32,30;,
 4;27,33,34,28;,
 4;27,29,35,33;,
 3;30,32,4;,
 3;30,4,31;,
 4;36,37,38,39;,
 4;36,40,41,37;,
 3;37,41,1;,
 3;37,1,38;,
 4;42,43,34,33;,
 4;39,43,42,36;,
 4;44,42,33,35;,
 4;36,42,44,40;,
 4;45,46,15,14;,
 4;12,46,45,13;,
 4;31,47,48,25;,
 3;49,47,50;,
 3;50,47,31;,
 4;25,48,51,22;,
 4;22,51,52,23;,
 4;53,52,54,55;,
 4;23,52,53,29;,
 4;56,53,55,57;,
 4;29,53,56,35;,
 4;58,56,57,59;,
 4;35,56,58,44;,
 4;60,58,59,61;,
 4;44,58,60,40;,
 4;62,60,61,63;,
 4;40,60,62,41;,
 3;41,62,64;,
 3;64,62,63;,
 4;65,66,3,16;,
 3;3,66,50;,
 3;50,66,49;,
 4;67,65,16,10;,
 4;68,67,10,7;,
 4;69,68,7,6;,
 4;54,68,69,55;,
 4;70,69,6,13;,
 4;55,69,70,57;,
 4;71,70,13,45;,
 4;57,70,71,59;,
 4;72,71,45,14;,
 4;59,71,72,61;,
 4;73,72,14,2;,
 4;61,72,73,63;,
 3;63,73,64;,
 3;64,73,2;,
 3;52,74,54;,
 3;51,74,52;,
 3;54,74,68;,
 3;68,74,67;,
 3;75,76,48;,
 3;48,76,51;,
 3;65,76,75;,
 3;67,76,65;,
 3;48,77,75;,
 3;47,77,48;,
 3;75,77,65;,
 3;65,77,66;,
 3;78,79,80;,
 3;80,81,82;,
 3;82,83,84;,
 3;84,85,86;,
 4;87,78,80,88;,
 4;88,80,82,89;,
 4;89,82,84,90;,
 4;90,84,86,91;,
 4;92,93,94,95;,
 4;95,94,96,97;,
 4;97,96,98,99;,
 4;99,98,100,101;,
 3;102,1,0;,
 3;5,4,103;,
 4;9,8,104,105;,
 4;8,11,106,104;,
 4;9,105,107,12;,
 4;102,0,15,108;,
 4;106,11,17,109;,
 4;17,5,103,109;,
 4;21,20,110,111;,
 4;110,112,113,111;,
 4;113,114,115,111;,
 4;115,26,21,111;,
 4;20,28,116,110;,
 4;116,117,112,110;,
 4;114,118,119,115;,
 4;119,32,26,115;,
 4;28,34,120,116;,
 4;120,121,117,116;,
 3;4,32,119;,
 3;118,4,119;,
 4;39,38,122,123;,
 4;122,124,125,123;,
 3;1,124,122;,
 3;38,1,122;,
 4;120,34,43,126;,
 4;123,126,43,39;,
 4;121,120,126,127;,
 4;125,127,126,123;,
 4;108,15,46,128;,
 4;107,128,46,12;,
 4;114,129,130,118;,
 3;50,130,131;,
 3;118,130,50;,
 4;113,132,129,114;,
 4;112,133,132,113;,
 4;134,135,133,136;,
 4;117,136,133,112;,
 4;137,134,136,138;,
 4;121,138,136,117;,
 4;139,137,138,140;,
 4;127,140,138,121;,
 4;141,139,140,142;,
 4;125,142,140,127;,
 4;143,141,142,144;,
 4;124,144,142,125;,
 3;64,144,124;,
 3;143,144,64;,
 4;109,103,145,146;,
 3;50,145,103;,
 3;131,145,50;,
 4;106,109,146,147;,
 4;104,106,147,148;,
 4;105,104,148,149;,
 4;134,149,148,135;,
 4;107,105,149,150;,
 4;137,150,149,134;,
 4;128,107,150,151;,
 4;139,151,150,137;,
 4;108,128,151,152;,
 4;141,152,151,139;,
 4;102,108,152,153;,
 4;143,153,152,141;,
 3;64,153,143;,
 3;102,153,64;,
 3;135,154,133;,
 3;133,154,132;,
 3;148,154,135;,
 3;147,154,148;,
 3;129,155,156;,
 3;132,155,129;,
 3;156,155,146;,
 3;146,155,147;,
 3;156,157,129;,
 3;129,157,130;,
 3;146,157,156;,
 3;145,157,146;,
 3;158,79,78;,
 3;159,81,158;,
 3;160,83,159;,
 3;86,85,160;,
 4;161,158,78,87;,
 4;162,159,158,161;,
 4;163,160,159,162;,
 4;91,86,160,163;,
 4;164,165,93,92;,
 4;166,167,165,164;,
 4;168,169,167,166;,
 4;101,100,169,168;;
 
 MeshMaterialList {
  2;
  176;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.736000;0.672000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  170;
  -0.000000;-0.997787;-0.066496;,
  -0.000000;-0.785798;-0.618483;,
  0.000000;0.772397;-0.635141;,
  -0.000000;0.836657;-0.547727;,
  -0.000000;-0.043448;-0.999056;,
  0.000000;0.147403;-0.989077;,
  0.000000;0.424144;-0.905595;,
  0.000000;-0.040728;-0.999170;,
  -0.000000;-0.327756;-0.944763;,
  0.000000;-0.782488;0.622666;,
  0.000000;0.689886;-0.723918;,
  0.000000;-0.038458;0.999260;,
  0.000000;0.110308;0.993897;,
  -0.000000;0.393339;0.919394;,
  -0.000000;-0.038350;0.999264;,
  0.000000;-0.322932;0.946422;,
  -0.000000;0.651133;-0.758964;,
  0.000000;0.676580;0.736369;,
  0.320656;-0.830038;-0.456307;,
  0.473603;-0.880739;0.000000;,
  0.287741;0.828012;-0.481249;,
  0.630580;-0.005852;-0.776102;,
  0.999501;0.031567;-0.000663;,
  0.599213;0.090840;-0.795419;,
  0.996921;-0.078410;-0.000385;,
  0.484492;0.416492;-0.769286;,
  0.618895;-0.002983;-0.785468;,
  0.996591;0.082505;-0.000283;,
  0.547849;-0.343084;-0.762990;,
  0.924589;-0.380966;-0.000000;,
  0.305069;-0.828678;0.469281;,
  -0.355066;0.728627;-0.585689;,
  0.620727;-0.015703;0.783870;,
  0.588823;0.095029;0.802656;,
  0.474784;0.410973;0.778255;,
  0.596865;-0.017035;0.802161;,
  0.517797;-0.344971;0.782867;,
  0.337126;0.733525;-0.590158;,
  0.458167;0.888078;-0.037428;,
  0.332608;0.734145;0.591948;,
  0.255445;0.406592;-0.877172;,
  0.308926;0.135147;-0.941435;,
  0.181955;0.650510;-0.737380;,
  0.308512;-0.032125;-0.950678;,
  0.141230;0.773111;-0.618347;,
  0.290257;-0.033848;-0.956350;,
  0.255254;-0.319402;-0.912594;,
  0.156143;-0.781479;-0.604078;,
  0.000000;0.999051;-0.043563;,
  0.000000;0.895019;-0.446028;,
  0.000000;0.918058;0.396447;,
  0.000000;0.808681;-0.588248;,
  0.000000;0.813580;0.581452;,
  0.000000;0.002024;-0.999998;,
  0.280198;0.004154;-0.959933;,
  0.000000;0.002040;0.999998;,
  0.595288;0.032154;-0.802869;,
  0.564088;0.024594;0.825349;,
  0.990604;0.136764;-0.000000;,
  0.464400;0.818813;-0.337458;,
  0.435517;0.826719;-0.356175;,
  0.734110;0.461645;-0.497962;,
  0.920441;-0.036513;-0.389173;,
  0.912600;0.020915;-0.408318;,
  0.891835;0.050525;-0.449530;,
  0.863613;0.091276;-0.495824;,
  0.811812;-0.350401;-0.467098;,
  0.458840;-0.858744;-0.228088;,
  0.459927;0.835705;0.300107;,
  0.453659;0.829708;0.325235;,
  0.754043;0.444968;0.483138;,
  0.932658;-0.027143;0.359739;,
  0.929647;0.019548;0.367933;,
  0.902072;0.041880;0.429549;,
  0.866243;0.085804;0.492200;,
  0.811812;-0.350401;0.467098;,
  0.458840;-0.858744;0.228088;,
  0.988357;-0.152149;-0.001049;,
  -0.556915;0.829557;-0.040994;,
  0.000000;0.206751;-0.978394;,
  0.625874;0.259571;-0.735462;,
  0.955165;0.296076;0.000000;,
  0.625876;0.259571;0.735461;,
  0.000000;0.206751;0.978394;,
  0.177072;0.927356;-0.329632;,
  0.284313;0.958480;-0.021947;,
  0.197608;0.938684;0.282530;,
  0.275640;0.859540;-0.430364;,
  0.441393;0.897293;-0.006095;,
  0.303878;0.854892;0.420497;,
  -0.320656;-0.830038;-0.456307;,
  -0.473603;-0.880739;0.000000;,
  -0.287741;0.828012;-0.481249;,
  -0.630580;-0.005852;-0.776102;,
  -0.999501;0.031567;-0.000663;,
  -0.599213;0.090840;-0.795419;,
  -0.996921;-0.078410;-0.000385;,
  -0.484492;0.416492;-0.769286;,
  -0.618895;-0.002983;-0.785468;,
  -0.996591;0.082505;-0.000283;,
  -0.547849;-0.343084;-0.762990;,
  -0.924589;-0.380966;-0.000000;,
  -0.305069;-0.828678;0.469281;,
  0.355066;0.728627;-0.585689;,
  -0.620727;-0.015703;0.783870;,
  -0.588823;0.095029;0.802656;,
  -0.474784;0.410973;0.778255;,
  -0.596865;-0.017035;0.802161;,
  -0.517797;-0.344971;0.782867;,
  -0.337126;0.733525;-0.590158;,
  -0.458167;0.888078;-0.037428;,
  -0.332608;0.734145;0.591948;,
  -0.255445;0.406592;-0.877172;,
  -0.308926;0.135147;-0.941435;,
  -0.181955;0.650510;-0.737380;,
  -0.308512;-0.032125;-0.950678;,
  -0.141230;0.773111;-0.618347;,
  -0.290257;-0.033848;-0.956350;,
  -0.255254;-0.319402;-0.912594;,
  -0.156143;-0.781479;-0.604078;,
  -0.280198;0.004154;-0.959933;,
  -0.595288;0.032154;-0.802869;,
  -0.564088;0.024594;0.825349;,
  -0.990604;0.136764;-0.000000;,
  -0.464400;0.818813;-0.337458;,
  -0.435517;0.826719;-0.356175;,
  -0.734110;0.461645;-0.497962;,
  -0.920441;-0.036513;-0.389173;,
  -0.912600;0.020915;-0.408318;,
  -0.891835;0.050525;-0.449530;,
  -0.863613;0.091276;-0.495824;,
  -0.811812;-0.350401;-0.467098;,
  -0.458840;-0.858744;-0.228088;,
  -0.459927;0.835705;0.300107;,
  -0.453659;0.829708;0.325235;,
  -0.754043;0.444968;0.483138;,
  -0.932658;-0.027143;0.359739;,
  -0.929647;0.019548;0.367933;,
  -0.902072;0.041880;0.429549;,
  -0.866243;0.085804;0.492200;,
  -0.811812;-0.350401;0.467098;,
  -0.458840;-0.858744;0.228088;,
  -0.988357;-0.152149;-0.001049;,
  0.556915;0.829557;-0.040994;,
  -0.625874;0.259571;-0.735462;,
  -0.955165;0.296076;0.000000;,
  -0.625876;0.259571;0.735461;,
  -0.177072;0.927356;-0.329632;,
  -0.284313;0.958480;-0.021947;,
  -0.197608;0.938684;0.282530;,
  -0.275640;0.859540;-0.430364;,
  -0.441393;0.897293;-0.006095;,
  -0.303878;0.854892;0.420497;,
  0.272724;0.844014;0.461803;,
  0.000000;0.806912;0.590672;,
  -0.359540;0.776097;0.518078;,
  0.000000;0.810058;0.586349;,
  0.000000;0.887223;0.461342;,
  -0.454641;0.782290;0.425821;,
  -0.518017;0.800427;0.301621;,
  -0.503675;0.777734;-0.376088;,
  0.346272;0.937990;-0.016445;,
  0.538926;0.841743;-0.032068;,
  -0.272724;0.844014;0.461803;,
  0.359540;0.776097;0.518078;,
  0.454641;0.782290;0.425821;,
  0.518017;0.800427;0.301621;,
  0.503675;0.777734;-0.376088;,
  -0.346272;0.937990;-0.016445;,
  -0.538926;0.841743;-0.032068;;
  176;
  3;9,0,30;,
  3;31,3,10;,
  4;32,33,12,11;,
  4;33,34,13,12;,
  4;14,35,32,11;,
  4;36,15,9,30;,
  4;39,17,13,34;,
  4;39,153,154,17;,
  4;40,41,5,6;,
  4;40,25,23,41;,
  4;40,42,37,25;,
  4;40,6,16,42;,
  4;41,43,4,5;,
  4;41,23,21,43;,
  4;42,44,20,37;,
  4;42,16,2,44;,
  4;43,45,7,4;,
  4;43,21,26,45;,
  3;155,156,157;,
  3;155,157,158;,
  4;46,47,1,8;,
  4;46,28,18,47;,
  3;47,18,0;,
  3;47,0,1;,
  4;54,53,7,45;,
  4;8,53,54,46;,
  4;56,54,45,26;,
  4;46,54,56,28;,
  4;57,55,15,36;,
  4;14,55,57,35;,
  4;20,59,60,37;,
  3;78,159,157;,
  3;157,159,158;,
  4;37,60,61,25;,
  4;25,61,62,23;,
  4;63,62,24,22;,
  4;23,62,63,21;,
  4;64,63,22,27;,
  4;21,63,64,26;,
  4;65,64,27,58;,
  4;26,64,65,56;,
  4;66,65,58,29;,
  4;56,65,66,28;,
  4;67,66,29,19;,
  4;28,66,67,18;,
  3;18,67,0;,
  3;0,67,19;,
  4;69,68,153,39;,
  3;31,160,3;,
  3;3,160,78;,
  4;70,69,39,34;,
  4;71,70,34,33;,
  4;72,71,33,32;,
  4;24,71,72,22;,
  4;73,72,32,35;,
  4;22,72,73,27;,
  4;74,73,35,57;,
  4;27,73,74,58;,
  4;75,74,57,36;,
  4;58,74,75,29;,
  4;76,75,36,30;,
  4;29,75,76,19;,
  3;19,76,0;,
  3;0,76,30;,
  3;62,77,24;,
  3;61,77,62;,
  3;24,77,71;,
  3;71,77,70;,
  3;38,161,60;,
  3;60,161,61;,
  3;69,161,38;,
  3;70,161,69;,
  3;60,162,38;,
  3;59,162,60;,
  3;38,162,69;,
  3;69,162,68;,
  3;49,48,84;,
  3;84,48,85;,
  3;85,48,86;,
  3;86,48,50;,
  4;51,49,84,87;,
  4;87,84,85,88;,
  4;88,85,86,89;,
  4;89,86,50,52;,
  4;79,79,80,80;,
  4;80,80,81,81;,
  4;81,81,82,82;,
  4;82,82,83,83;,
  3;102,0,9;,
  3;10,3,103;,
  4;11,12,105,104;,
  4;12,13,106,105;,
  4;11,104,107,14;,
  4;102,9,15,108;,
  4;106,13,17,111;,
  4;17,154,163,111;,
  4;6,5,113,112;,
  4;113,95,97,112;,
  4;97,109,114,112;,
  4;114,16,6,112;,
  4;5,4,115,113;,
  4;115,93,95,113;,
  4;109,92,116,114;,
  4;116,2,16,114;,
  4;4,7,117,115;,
  4;117,98,93,115;,
  3;157,156,164;,
  3;165,157,164;,
  4;8,1,119,118;,
  4;119,90,100,118;,
  3;0,90,119;,
  3;1,0,119;,
  4;117,7,53,120;,
  4;118,120,53,8;,
  4;98,117,120,121;,
  4;100,121,120,118;,
  4;108,15,55,122;,
  4;107,122,55,14;,
  4;109,125,124,92;,
  3;157,166,143;,
  3;165,166,157;,
  4;97,126,125,109;,
  4;95,127,126,97;,
  4;94,96,127,128;,
  4;93,128,127,95;,
  4;99,94,128,129;,
  4;98,129,128,93;,
  4;123,99,129,130;,
  4;121,130,129,98;,
  4;101,123,130,131;,
  4;100,131,130,121;,
  4;91,101,131,132;,
  4;90,132,131,100;,
  3;0,132,90;,
  3;91,132,0;,
  4;111,163,133,134;,
  3;3,167,103;,
  3;143,167,3;,
  4;106,111,134,135;,
  4;105,106,135,136;,
  4;104,105,136,137;,
  4;94,137,136,96;,
  4;107,104,137,138;,
  4;99,138,137,94;,
  4;122,107,138,139;,
  4;123,139,138,99;,
  4;108,122,139,140;,
  4;101,140,139,123;,
  4;102,108,140,141;,
  4;91,141,140,101;,
  3;0,141,91;,
  3;102,141,0;,
  3;96,142,127;,
  3;127,142,126;,
  3;136,142,96;,
  3;135,142,136;,
  3;125,168,110;,
  3;126,168,125;,
  3;110,168,134;,
  3;134,168,135;,
  3;110,169,125;,
  3;125,169,124;,
  3;134,169,110;,
  3;133,169,134;,
  3;147,48,49;,
  3;148,48,147;,
  3;149,48,148;,
  3;50,48,149;,
  4;150,147,49,51;,
  4;151,148,147,150;,
  4;152,149,148,151;,
  4;52,50,149,152;,
  4;144,144,79,79;,
  4;145,145,144,144;,
  4;146,146,145,145;,
  4;83,83,146,146;;
 }
 MeshTextureCoords {
  170;
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.562500;1.000000;,
  0.500000;1.000000;,
  0.625000;0.500000;,
  0.625000;0.750000;,
  0.500000;0.750000;,
  0.500000;0.500000;,
  0.625000;0.875000;,
  0.500000;0.875000;,
  0.500000;0.263320;,
  0.625000;0.263320;,
  0.625000;0.022200;,
  0.500000;0.022200;,
  0.625000;0.982380;,
  0.500000;0.982380;,
  0.576680;0.875000;,
  0.576680;0.750000;,
  0.500000;0.750000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.625000;0.750000;,
  0.576680;0.982380;,
  0.625000;0.982380;,
  0.500000;0.982380;,
  0.576680;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.576680;1.000000;,
  0.625000;1.000000;,
  0.500000;1.000000;,
  0.576680;0.263320;,
  0.500000;0.263320;,
  0.625000;0.263320;,
  0.576680;0.022200;,
  0.576680;0.000000;,
  0.500000;0.000000;,
  0.500000;0.022200;,
  0.625000;0.022200;,
  0.625000;0.000000;,
  0.576680;0.142780;,
  0.500000;0.142780;,
  0.625000;0.142780;,
  0.625000;0.142780;,
  0.500000;0.142780;,
  0.687500;1.000000;,
  0.687500;0.982380;,
  0.750000;1.000000;,
  0.687500;1.000000;,
  0.687500;0.875000;,
  0.687500;0.750000;,
  0.687500;0.500000;,
  0.750000;0.750000;,
  0.750000;0.500000;,
  0.687500;0.263320;,
  0.750000;0.263320;,
  0.687500;0.142780;,
  0.750000;0.142780;,
  0.687500;0.022200;,
  0.750000;0.022200;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.687500;0.000000;,
  0.687500;0.982380;,
  0.687500;1.000000;,
  0.687500;0.875000;,
  0.687500;0.750000;,
  0.687500;0.500000;,
  0.687500;0.263320;,
  0.687500;0.142780;,
  0.687500;0.022200;,
  0.687500;0.000000;,
  0.750000;0.812500;,
  0.750000;0.982380;,
  0.750000;0.928690;,
  0.750000;0.991190;,
  0.000000;0.125000;,
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.000000;0.250000;,
  0.125000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.625000;0.750000;,
  0.625000;0.500000;,
  0.625000;0.875000;,
  0.625000;0.263320;,
  0.625000;0.022200;,
  0.625000;0.982380;,
  0.576680;0.750000;,
  0.576680;0.875000;,
  0.625000;0.750000;,
  0.625000;0.875000;,
  0.625000;0.982380;,
  0.576680;0.982380;,
  0.576680;0.500000;,
  0.625000;0.500000;,
  0.625000;1.000000;,
  0.576680;1.000000;,
  0.576680;0.263320;,
  0.625000;0.263320;,
  0.576680;0.000000;,
  0.576680;0.022200;,
  0.625000;0.000000;,
  0.625000;0.022200;,
  0.576680;0.142780;,
  0.625000;0.142780;,
  0.625000;0.142780;,
  0.687500;0.982380;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.687500;0.875000;,
  0.687500;0.750000;,
  0.750000;0.500000;,
  0.750000;0.750000;,
  0.687500;0.500000;,
  0.750000;0.263320;,
  0.687500;0.263320;,
  0.750000;0.142780;,
  0.687500;0.142780;,
  0.750000;0.022200;,
  0.687500;0.022200;,
  0.750000;0.000000;,
  0.687500;0.000000;,
  0.687500;1.000000;,
  0.687500;0.982380;,
  0.687500;0.875000;,
  0.687500;0.750000;,
  0.687500;0.500000;,
  0.687500;0.263320;,
  0.687500;0.142780;,
  0.687500;0.022200;,
  0.687500;0.000000;,
  0.750000;0.812500;,
  0.750000;0.928690;,
  0.750000;0.982380;,
  0.750000;0.991190;,
  0.125000;0.125000;,
  0.250000;0.125000;,
  0.375000;0.125000;,
  0.125000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.375000;1.000000;,
  0.375000;0.000000;;
 }
}

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
 70;
 -17.75468;-0.12753;-3.38564;,
 -16.92846;-0.18612;0.00000;,
 -17.84491;2.21839;-2.39402;,
 -16.92846;-0.18612;0.00000;,
 -17.88230;3.19010;0.00000;,
 -16.92846;-0.18612;0.00000;,
 -17.84491;2.21839;2.39402;,
 -16.92846;-0.18612;0.00000;,
 -17.75468;-0.12753;3.38564;,
 -16.92846;-0.18612;0.00000;,
 -17.66453;-2.47343;2.39402;,
 -16.92846;-0.18612;0.00000;,
 -17.62719;-3.44518;0.00000;,
 -16.92846;-0.18612;0.00000;,
 -17.66453;-2.47343;-2.39402;,
 -16.92846;-0.18612;0.00000;,
 -17.75468;-0.12753;-3.38564;,
 -1.65458;0.05533;-3.72217;,
 -1.69385;-2.37015;-2.63198;,
 -7.84435;-2.49624;-2.61887;,
 -7.93366;0.07002;-3.70366;,
 -1.66136;-3.43843;0.00000;,
 -7.80736;-3.55923;0.00000;,
 -1.65197;2.57908;-2.63198;,
 -1.65458;0.05533;-3.72217;,
 -7.93366;0.07002;-3.70366;,
 -7.89619;2.63878;-2.61887;,
 -1.68916;-2.52475;2.63198;,
 -7.84435;-2.49624;2.61887;,
 -1.64115;3.64768;0.00000;,
 -7.75264;3.70532;0.00000;,
 -1.65458;0.05533;3.72217;,
 -7.93366;0.07002;3.70366;,
 -1.65197;2.57908;2.63198;,
 -7.89619;2.63878;2.61887;,
 -12.61867;-2.54495;-2.56714;,
 -12.71538;-0.02938;-3.63049;,
 -12.57863;-3.58693;0.00000;,
 -12.71538;-0.02938;-3.63049;,
 -12.81209;2.48621;-2.56714;,
 -12.61867;-2.54495;2.56714;,
 -12.85216;3.52820;0.00000;,
 -12.71538;-0.02938;3.63049;,
 -12.81209;2.48621;2.56714;,
 -2.45326;1.30189;-1.31197;,
 -2.88599;-0.00022;0.00000;,
 -2.41416;0.01627;-1.85540;,
 -2.46947;1.83440;0.00000;,
 -2.88599;-0.00022;0.00000;,
 -2.45326;1.30189;1.31197;,
 -2.88599;-0.00022;0.00000;,
 -2.41416;0.01627;1.85540;,
 -2.88599;-0.00022;0.00000;,
 -2.37506;-1.26936;1.31197;,
 -2.88599;-0.00022;0.00000;,
 -2.35886;-1.80185;0.00000;,
 -2.88599;-0.00022;0.00000;,
 -2.37506;-1.26936;-1.31197;,
 -2.88599;-0.00022;0.00000;,
 -2.41416;0.01627;-1.85540;,
 -2.88599;-0.00022;0.00000;,
 -1.65197;2.57908;-2.63198;,
 -1.65458;0.05533;-3.72217;,
 -1.64115;3.64768;0.00000;,
 -1.65197;2.57908;2.63198;,
 -1.65458;0.05533;3.72217;,
 -1.68916;-2.52475;2.63198;,
 -1.66136;-3.43843;0.00000;,
 -1.69385;-2.37015;-2.63198;,
 -1.65458;0.05533;-3.72217;;
 
 48;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;6,7,8;,
 3;8,9,10;,
 3;10,11,12;,
 3;12,13,14;,
 3;14,15,16;,
 4;17,18,19,20;,
 4;21,22,19,18;,
 4;23,24,25,26;,
 4;27,28,22,21;,
 4;29,23,26,30;,
 4;31,32,28,27;,
 4;33,29,30,34;,
 4;33,34,32,31;,
 4;20,19,35,36;,
 4;16,36,35,14;,
 4;12,14,35,37;,
 4;22,37,35,19;,
 4;26,25,38,39;,
 4;2,39,38,0;,
 4;10,12,37,40;,
 4;28,40,37,22;,
 4;30,26,39,41;,
 4;4,41,39,2;,
 4;8,10,40,42;,
 4;32,42,40,28;,
 4;34,30,41,43;,
 4;6,43,41,4;,
 4;6,8,42,43;,
 4;34,43,42,32;,
 3;44,45,46;,
 3;47,48,44;,
 3;49,50,47;,
 3;51,52,49;,
 3;53,54,51;,
 3;55,56,53;,
 3;57,58,55;,
 3;59,60,57;,
 4;61,44,46,62;,
 4;63,47,44,61;,
 4;64,49,47,63;,
 4;65,51,49,64;,
 4;66,53,51,65;,
 4;67,55,53,66;,
 4;68,57,55,67;,
 4;69,59,57,68;;
 
 MeshMaterialList {
  1;
  48;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  }
 }
 MeshNormals {
  58;
  -0.970995;-0.036277;0.236330;,
  -0.965046;-0.203977;0.164558;,
  -0.962333;-0.271875;0.000000;,
  -0.965046;-0.203977;-0.164558;,
  -0.970995;-0.036277;-0.236330;,
  -0.976004;0.136580;-0.169593;,
  -0.977781;0.209631;0.000000;,
  -0.976004;0.136580;0.169593;,
  -0.999397;-0.034732;-0.000000;,
  0.005070;-0.714077;-0.700049;,
  -0.007280;-0.002022;-0.999971;,
  0.009606;-0.999933;-0.006472;,
  -0.012441;0.712520;-0.701541;,
  0.000657;-0.716655;0.697428;,
  -0.012972;0.999916;0.000000;,
  -0.008596;0.000764;0.999963;,
  -0.012441;0.712520;0.701541;,
  -0.017245;-0.712400;-0.701562;,
  -0.031846;-0.001041;-0.999492;,
  -0.011136;-0.999938;-0.000000;,
  -0.045681;0.710804;-0.701905;,
  -0.017245;-0.712400;0.701562;,
  -0.051034;0.998697;-0.000000;,
  -0.031846;-0.001041;0.999492;,
  -0.045681;0.710804;0.701905;,
  0.999533;0.030566;-0.000000;,
  0.949860;0.017232;0.312200;,
  0.949168;-0.216049;0.228917;,
  0.948306;-0.317359;-0.000000;,
  0.949168;-0.216049;-0.228917;,
  0.950326;0.015757;-0.310857;,
  0.947819;0.239963;-0.209898;,
  0.942172;0.335113;0.003227;,
  0.946027;0.243030;0.214403;,
  0.000647;-0.003695;-0.999993;,
  0.004544;0.713160;-0.700986;,
  0.009068;0.999959;0.000000;,
  0.004544;0.713160;0.700986;,
  -0.002005;0.001884;0.999996;,
  0.001656;-0.721096;0.692833;,
  0.013388;-0.999827;-0.012911;,
  0.010465;-0.715916;-0.698108;,
  -0.048497;-0.001727;-0.998822;,
  -0.034145;-0.712388;-0.700955;,
  -0.028070;-0.999606;0.000000;,
  -0.061909;0.709747;-0.701731;,
  -0.034145;-0.712388;0.700955;,
  -0.067062;0.997749;0.000000;,
  -0.048497;-0.001727;0.998822;,
  -0.061909;0.709747;0.701731;,
  0.916692;-0.282886;0.282227;,
  0.926297;0.004787;0.376765;,
  0.911632;-0.411007;-0.000000;,
  0.916692;-0.282886;-0.282227;,
  0.927387;0.001842;-0.374098;,
  0.930471;0.272305;-0.245099;,
  0.921397;0.388568;0.006458;,
  0.926308;0.278290;0.253984;;
  48;
  3;0,8,1;,
  3;1,8,2;,
  3;2,8,3;,
  3;3,8,4;,
  3;4,8,5;,
  3;5,8,6;,
  3;6,8,7;,
  3;7,8,0;,
  4;34,41,9,10;,
  4;40,11,9,41;,
  4;35,34,10,12;,
  4;39,13,11,40;,
  4;36,35,12,14;,
  4;38,15,13,39;,
  4;37,36,14,16;,
  4;37,16,15,38;,
  4;10,9,17,18;,
  4;42,18,17,43;,
  4;44,43,17,19;,
  4;11,19,17,9;,
  4;12,10,18,20;,
  4;45,20,18,42;,
  4;46,44,19,21;,
  4;13,21,19,11;,
  4;14,12,20,22;,
  4;47,22,20,45;,
  4;48,46,21,23;,
  4;15,23,21,13;,
  4;16,14,22,24;,
  4;49,24,22,47;,
  4;49,48,23,24;,
  4;16,24,23,15;,
  3;27,25,26;,
  3;28,25,27;,
  3;29,25,28;,
  3;30,25,29;,
  3;31,25,30;,
  3;32,25,31;,
  3;33,25,32;,
  3;26,25,33;,
  4;50,27,26,51;,
  4;52,28,27,50;,
  4;53,29,28,52;,
  4;54,30,29,53;,
  4;55,31,30,54;,
  4;56,32,31,55;,
  4;57,33,32,56;,
  4;51,26,33,57;;
 }
 MeshTextureCoords {
  70;
  0.000000;1.000000;,
  0.062500;1.000000;,
  0.125000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.875000;0.000000;,
  0.875000;0.384250;,
  1.000000;0.384250;,
  0.750000;0.000000;,
  0.750000;0.384250;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.384250;,
  0.125000;0.384250;,
  0.625000;0.000000;,
  0.625000;0.384250;,
  0.250000;0.000000;,
  0.250000;0.384250;,
  0.500000;0.000000;,
  0.500000;0.384250;,
  0.375000;0.000000;,
  0.375000;0.384250;,
  0.875000;0.672230;,
  1.000000;0.672230;,
  0.750000;0.672230;,
  0.000000;0.672230;,
  0.125000;0.672230;,
  0.625000;0.672230;,
  0.250000;0.672230;,
  0.500000;0.672230;,
  0.375000;0.672230;,
  0.125000;0.125000;,
  0.062500;0.000000;,
  0.000000;0.125000;,
  0.250000;0.125000;,
  0.187500;0.000000;,
  0.375000;0.125000;,
  0.312500;0.000000;,
  0.500000;0.125000;,
  0.437500;0.000000;,
  0.625000;0.125000;,
  0.562500;0.000000;,
  0.750000;0.125000;,
  0.687500;0.000000;,
  0.875000;0.125000;,
  0.812500;0.000000;,
  1.000000;0.125000;,
  0.937500;0.000000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;;
 }
}

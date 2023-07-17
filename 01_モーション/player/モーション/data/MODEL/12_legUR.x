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
 95;
 -3.48254;-1.41051;3.40294;,
 -3.24886;-7.04086;3.17146;,
 -0.07742;-7.04086;4.48512;,
 -0.07958;-1.41051;4.81250;,
 -4.95677;-1.41051;0.00000;,
 -4.56254;-7.04086;0.00000;,
 -3.48254;-1.41051;-3.40296;,
 -3.24886;-7.04086;-3.17146;,
 -0.07958;-1.41051;-4.81250;,
 -0.07742;-7.04086;-4.48512;,
 3.32336;-1.41051;-3.40296;,
 3.09403;-7.04086;-3.17146;,
 4.73291;-1.41051;-0.00000;,
 4.40770;-7.04086;-0.00000;,
 3.32336;-1.41051;3.40294;,
 3.09403;-7.04086;3.17146;,
 -0.07958;-1.41051;4.81250;,
 -0.07742;-7.04086;4.48512;,
 -3.52885;0.80290;3.30899;,
 -0.07278;0.80290;4.67963;,
 -5.12920;0.91341;0.00000;,
 -3.52885;0.80290;-3.30899;,
 -0.07278;0.80290;-4.67963;,
 3.23621;0.80290;-3.30899;,
 4.60685;0.80290;0.00000;,
 3.23621;0.80290;3.30899;,
 -0.07278;0.80290;4.67963;,
 -2.36704;1.94714;1.88240;,
 -0.07278;1.94714;2.66211;,
 -3.41830;1.94714;0.00000;,
 -2.36704;1.94714;-1.88240;,
 -0.07278;1.94714;-2.66211;,
 1.80962;1.94714;-1.88240;,
 2.58934;1.94714;0.00000;,
 1.80962;1.94714;1.88240;,
 -0.07278;1.94714;2.66211;,
 -0.87325;2.52646;0.00000;,
 -0.87325;2.52646;0.00000;,
 -0.87325;2.52646;0.00000;,
 -0.87325;2.52646;0.00000;,
 -0.87325;2.52646;0.00000;,
 -0.87325;2.52646;0.00000;,
 -0.87325;2.52646;0.00000;,
 -0.87325;2.52646;0.00000;,
 -0.07742;-7.04086;-4.48512;,
 3.09403;-7.04086;-3.17146;,
 2.86542;-12.82118;-2.94285;,
 -0.07742;-12.82118;-4.16181;,
 -0.07742;-16.31672;-4.01902;,
 2.76445;-16.31672;-2.84187;,
 3.94160;-16.31672;-0.00000;,
 4.08438;-12.82118;-0.00000;,
 4.40770;-7.04086;-0.00000;,
 -3.24886;-7.04086;-3.17146;,
 -3.02027;-12.82118;-2.94285;,
 -2.91930;-16.31672;-2.84187;,
 2.76445;-16.31672;2.84187;,
 2.86542;-12.82118;2.94285;,
 3.09403;-7.04086;3.17146;,
 -4.56254;-7.04086;0.00000;,
 -4.23923;-12.82118;0.00000;,
 -4.09643;-16.31672;0.00000;,
 -0.07742;-16.31672;4.01902;,
 -0.07742;-12.82118;4.16181;,
 -0.07742;-7.04086;4.48512;,
 -3.24886;-7.04086;3.17146;,
 -3.02027;-12.82118;2.94285;,
 -2.91930;-16.31672;2.84187;,
 -0.07742;-16.31672;4.01902;,
 -0.07742;-12.82118;4.16181;,
 -0.07742;-7.04086;4.48512;,
 1.34351;-18.31360;-1.42094;,
 -0.07742;-19.37154;0.00000;,
 -0.07742;-18.31360;-2.00951;,
 1.93209;-18.31360;-0.00000;,
 -0.07742;-19.37154;0.00000;,
 1.34351;-18.31360;-1.42094;,
 -1.49836;-18.31360;-1.42094;,
 -0.07742;-18.31360;-2.00951;,
 -0.07742;-19.37154;0.00000;,
 -2.08693;-18.31360;0.00000;,
 -1.49836;-18.31360;-1.42094;,
 -0.07742;-19.37154;0.00000;,
 -1.49836;-18.31360;1.42094;,
 -2.08693;-18.31360;0.00000;,
 -0.07742;-19.37154;0.00000;,
 -0.07742;-18.31360;2.00951;,
 -1.49836;-18.31360;1.42094;,
 -0.07742;-19.37154;0.00000;,
 1.34351;-18.31360;1.42094;,
 -0.07742;-18.31360;2.00951;,
 -0.07742;-19.37154;0.00000;,
 1.34351;-18.31360;1.42094;,
 -0.07742;-19.37154;0.00000;,
 1.93209;-18.31360;-0.00000;;
 
 64;
 4;0,1,2,3;,
 4;4,5,1,0;,
 4;6,7,5,4;,
 4;8,9,7,6;,
 4;10,11,9,8;,
 4;12,13,11,10;,
 4;14,15,13,12;,
 4;16,17,15,14;,
 4;18,0,3,19;,
 4;20,4,0,18;,
 4;21,6,4,20;,
 4;22,8,6,21;,
 4;23,10,8,22;,
 4;24,12,10,23;,
 4;25,14,12,24;,
 4;26,16,14,25;,
 4;27,18,19,28;,
 4;29,20,18,27;,
 4;30,21,20,29;,
 4;31,22,21,30;,
 4;32,23,22,31;,
 4;33,24,23,32;,
 4;34,25,24,33;,
 4;35,26,25,34;,
 3;27,28,36;,
 3;29,27,37;,
 3;30,29,38;,
 3;31,30,39;,
 3;32,31,40;,
 3;33,32,41;,
 3;34,33,42;,
 3;35,34,43;,
 4;44,45,46,47;,
 4;48,47,46,49;,
 4;50,49,46,51;,
 4;52,51,46,45;,
 4;53,44,47,54;,
 4;55,54,47,48;,
 4;56,50,51,57;,
 4;58,57,51,52;,
 4;59,53,54,60;,
 4;61,60,54,55;,
 4;62,56,57,63;,
 4;64,63,57,58;,
 4;65,59,60,66;,
 4;67,66,60,61;,
 4;67,68,69,66;,
 4;65,66,69,70;,
 3;71,72,73;,
 3;74,75,76;,
 3;77,78,79;,
 3;80,81,82;,
 3;83,84,85;,
 3;86,87,88;,
 3;89,90,91;,
 3;92,93,94;,
 4;74,76,49,50;,
 4;50,56,92,94;,
 4;89,56,62,90;,
 4;67,87,86,68;,
 4;83,67,61,84;,
 4;55,81,80,61;,
 4;48,78,77,55;,
 4;48,49,71,73;;
 
 MeshMaterialList {
  1;
  64;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.096000;0.104000;0.104000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  60;
  -0.000000;-1.000000;0.000000;,
  0.000000;-0.409742;0.912202;,
  -0.645025;-0.409742;0.645023;,
  -0.912201;-0.409742;0.000000;,
  -0.645025;-0.409742;-0.645023;,
  0.000000;-0.409742;-0.912202;,
  0.645024;-0.409741;-0.645024;,
  0.912202;-0.409742;-0.000000;,
  0.645024;-0.409741;0.645024;,
  0.000001;-0.056946;0.998377;,
  -0.704454;-0.058402;0.707343;,
  -0.998200;-0.059977;0.000000;,
  -0.704454;-0.058403;-0.707343;,
  0.000001;-0.056947;-0.998377;,
  0.705965;-0.056811;-0.705964;,
  0.998388;-0.056755;0.000000;,
  0.705966;-0.056811;0.705964;,
  0.001846;-0.002421;0.999995;,
  -0.697655;-0.027859;0.715892;,
  -0.998692;-0.051133;0.000000;,
  -0.697655;-0.027859;-0.715892;,
  0.001846;-0.002419;-0.999995;,
  0.707114;-0.000008;-0.707100;,
  1.000000;-0.000401;0.000000;,
  0.707114;-0.000009;0.707099;,
  0.006560;0.534689;0.845024;,
  -0.699289;0.329690;0.634271;,
  -0.999279;-0.037972;0.000000;,
  -0.587447;0.503552;-0.633515;,
  0.006560;0.534690;-0.845023;,
  0.594462;0.541522;-0.594448;,
  0.840919;0.541161;0.000000;,
  0.594463;0.541521;0.594449;,
  0.003546;0.932463;0.361248;,
  -0.250153;0.923832;0.289753;,
  -0.368688;0.929553;0.000000;,
  -0.250153;0.923832;-0.289753;,
  0.003546;0.932463;-0.361248;,
  0.237734;0.940840;-0.241458;,
  0.332774;0.943007;0.000000;,
  0.237734;0.940840;0.241458;,
  -0.014612;0.999893;0.000000;,
  0.706280;-0.048332;-0.706280;,
  -0.000000;-0.048332;-0.998831;,
  0.998831;-0.048332;-0.000000;,
  -0.706281;-0.048332;-0.706280;,
  0.706280;-0.048332;0.706280;,
  -0.998831;-0.048333;-0.000000;,
  -0.000000;-0.048332;0.998831;,
  -0.706281;-0.048332;0.706280;,
  0.000000;-0.807011;-0.590537;,
  0.417573;-0.807010;-0.417573;,
  0.590537;-0.807011;-0.000000;,
  -0.417574;-0.807010;-0.417572;,
  -0.590538;-0.807010;0.000000;,
  -0.417574;-0.807010;0.417572;,
  0.000000;-0.807011;0.590537;,
  0.417573;-0.807010;0.417573;,
  -0.185006;0.843690;0.503945;,
  -0.510812;0.859692;0.000000;;
  64;
  4;18,10,9,17;,
  4;19,11,10,18;,
  4;20,12,11,19;,
  4;21,13,12,20;,
  4;22,14,13,21;,
  4;23,15,14,22;,
  4;24,16,15,23;,
  4;17,9,16,24;,
  4;26,18,17,25;,
  4;27,19,18,26;,
  4;28,20,19,27;,
  4;29,21,20,28;,
  4;30,22,21,29;,
  4;31,23,22,30;,
  4;32,24,23,31;,
  4;25,17,24,32;,
  4;34,58,25,33;,
  4;35,59,26,34;,
  4;36,28,59,35;,
  4;37,29,28,36;,
  4;38,30,29,37;,
  4;39,31,30,38;,
  4;40,32,31,39;,
  4;33,25,32,40;,
  3;34,33,41;,
  3;35,34,41;,
  3;36,35,41;,
  3;37,36,41;,
  3;38,37,41;,
  3;39,38,41;,
  3;40,39,41;,
  3;33,40,41;,
  4;13,14,42,43;,
  4;5,43,42,6;,
  4;7,6,42,44;,
  4;15,44,42,14;,
  4;12,13,43,45;,
  4;4,45,43,5;,
  4;8,7,44,46;,
  4;16,46,44,15;,
  4;11,12,45,47;,
  4;3,47,45,4;,
  4;1,8,46,48;,
  4;9,48,46,16;,
  4;10,11,47,49;,
  4;2,49,47,3;,
  4;2,1,48,49;,
  4;10,49,48,9;,
  3;51,0,50;,
  3;52,0,51;,
  3;53,50,0;,
  3;54,53,0;,
  3;55,54,0;,
  3;56,55,0;,
  3;57,56,0;,
  3;57,0,52;,
  4;52,51,6,7;,
  4;7,8,57,52;,
  4;57,8,1,56;,
  4;2,55,56,1;,
  4;55,2,3,54;,
  4;4,53,54,3;,
  4;5,50,53,4;,
  4;5,6,51,50;;
 }
 MeshTextureCoords {
  95;
  0.125000;0.625000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.250000;0.500000;,
  0.375000;0.625000;,
  0.375000;0.500000;,
  0.500000;0.625000;,
  0.500000;0.500000;,
  0.625000;0.625000;,
  0.625000;0.500000;,
  0.750000;0.625000;,
  0.750000;0.500000;,
  0.875000;0.625000;,
  0.875000;0.500000;,
  1.000000;0.625000;,
  1.000000;0.500000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.625000;0.500000;,
  0.500000;0.500000;,
  0.500000;0.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.750000;0.500000;,
  0.750000;1.000000;,
  0.375000;1.000000;,
  0.375000;0.500000;,
  0.375000;0.000000;,
  0.875000;0.000000;,
  0.875000;0.500000;,
  0.875000;1.000000;,
  0.250000;1.000000;,
  0.250000;0.500000;,
  0.250000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.125000;1.000000;,
  0.125000;0.500000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  0.593750;0.000000;,
  0.562500;0.000000;,
  0.531250;0.000000;,
  0.718750;0.000000;,
  0.687500;0.000000;,
  0.656250;0.000000;,
  0.406250;0.000000;,
  0.468750;0.000000;,
  0.437500;0.000000;,
  0.281250;0.000000;,
  0.343750;0.000000;,
  0.312500;0.000000;,
  0.156250;0.000000;,
  0.218750;0.000000;,
  0.187500;0.000000;,
  0.031250;0.000000;,
  0.093750;0.000000;,
  0.062500;0.000000;,
  0.906250;0.000000;,
  0.968750;0.000000;,
  0.937500;0.000000;,
  0.843750;0.000000;,
  0.812500;0.000000;,
  0.781250;0.000000;;
 }
}

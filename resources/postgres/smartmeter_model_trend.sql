\c smartmeter

DROP TABLE IF EXISTS "model" CASCADE;

CREATE TABLE "model" (
  doy INTEGER PRIMARY KEY,
  plan_id INTEGER,
  energy DOUBLE PRECISION
);

--- Model based on a four person household with an energy
--- consumption of 3650 kWh/year. Electricity tariff 
--- 29.38 ct/kWh and a service price of 175.91 €/year

INSERT INTO model (doy, plan_id, energy) VALUES ( 1, 6, 11.942 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 2, 6, 11.907 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 3, 6, 11.872 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 4, 6, 11.837 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 5, 6, 11.802 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 6, 6, 11.768 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 7, 6, 11.733 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 8, 6, 11.699 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 9, 6, 11.666 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 10, 6, 11.632 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 11, 6, 11.599 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 12, 6, 11.565 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 13, 6, 11.532 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 14, 6, 11.499 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 15, 6, 11.467 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 16, 6, 11.434 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 17, 6, 11.402 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 18, 6, 11.370 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 19, 6, 11.338 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 20, 6, 11.307 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 21, 6, 11.275 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 22, 6, 11.244 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 23, 6, 11.213 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 24, 6, 11.182 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 25, 6, 11.152 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 26, 6, 11.122 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 27, 6, 11.091 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 28, 6, 11.061 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 29, 6, 11.032 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 30, 6, 11.002 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 31, 6, 10.973 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 32, 6, 10.944 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 33, 6, 10.915 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 34, 6, 10.886 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 35, 6, 10.857 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 36, 6, 10.829 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 37, 6, 10.801 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 38, 6, 10.773 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 39, 6, 10.745 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 40, 6, 10.718 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 41, 6, 10.690 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 42, 6, 10.663 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 43, 6, 10.636 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 44, 6, 10.609 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 45, 6, 10.583 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 46, 6, 10.557 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 47, 6, 10.530 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 48, 6, 10.505 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 49, 6, 10.479 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 50, 6, 10.453 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 51, 6, 10.428 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 52, 6, 10.403 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 53, 6, 10.378 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 54, 6, 10.353 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 55, 6, 10.329 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 56, 6, 10.305 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 57, 6, 10.280 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 58, 6, 10.257 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 59, 6, 10.233 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 60, 6, 10.209 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 61, 6, 10.186 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 62, 6, 10.163 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 63, 6, 10.140 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 64, 6, 10.118 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 65, 6, 10.095 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 66, 6, 10.073 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 67, 6, 10.051 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 68, 6, 10.029 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 69, 6, 10.007 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 70, 6, 9.986 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 71, 6, 9.965 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 72, 6, 9.944 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 73, 6, 9.923 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 74, 6, 9.902 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 75, 6, 9.882 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 76, 6, 9.861 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 77, 6, 9.841 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 78, 6, 9.822 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 79, 6, 9.802 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 80, 6, 9.783 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 81, 6, 9.763 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 82, 6, 9.744 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 83, 6, 9.726 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 84, 6, 9.707 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 85, 6, 9.689 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 86, 6, 9.670 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 87, 6, 9.652 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 88, 6, 9.635 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 89, 6, 9.617 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 90, 6, 9.600 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 91, 6, 9.582 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 92, 6, 9.565 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 93, 6, 9.549 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 94, 6, 9.532 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 95, 6, 9.516 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 96, 6, 9.500 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 97, 6, 9.484 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 98, 6, 9.468 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 99, 6, 9.452 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 100, 6, 9.437 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 101, 6, 9.422 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 102, 6, 9.407 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 103, 6, 9.392 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 104, 6, 9.378 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 105, 6, 9.363 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 106, 6, 9.349 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 107, 6, 9.335 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 108, 6, 9.321 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 109, 6, 9.308 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 110, 6, 9.295 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 111, 6, 9.281 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 112, 6, 9.269 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 113, 6, 9.256 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 114, 6, 9.243 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 115, 6, 9.231 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 116, 6, 9.219 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 117, 6, 9.207 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 118, 6, 9.195 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 119, 6, 9.184 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 120, 6, 9.173 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 121, 6, 9.161 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 122, 6, 9.151 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 123, 6, 9.140 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 124, 6, 9.129 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 125, 6, 9.119 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 126, 6, 9.109 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 127, 6, 9.099 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 128, 6, 9.089 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 129, 6, 9.080 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 130, 6, 9.071 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 131, 6, 9.062 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 132, 6, 9.053 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 133, 6, 9.044 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 134, 6, 9.036 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 135, 6, 9.027 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 136, 6, 9.019 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 137, 6, 9.012 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 138, 6, 9.004 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 139, 6, 8.997 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 140, 6, 8.989 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 141, 6, 8.982 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 142, 6, 8.975 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 143, 6, 8.969 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 144, 6, 8.962 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 145, 6, 8.956 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 146, 6, 8.950 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 147, 6, 8.944 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 148, 6, 8.939 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 149, 6, 8.933 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 150, 6, 8.928 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 151, 6, 8.923 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 152, 6, 8.918 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 153, 6, 8.914 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 154, 6, 8.909 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 155, 6, 8.905 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 156, 6, 8.901 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 157, 6, 8.897 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 158, 6, 8.894 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 159, 6, 8.891 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 160, 6, 8.887 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 161, 6, 8.884 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 162, 6, 8.882 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 163, 6, 8.879 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 164, 6, 8.877 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 165, 6, 8.875 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 166, 6, 8.873 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 167, 6, 8.871 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 168, 6, 8.869 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 169, 6, 8.868 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 170, 6, 8.867 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 171, 6, 8.866 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 172, 6, 8.865 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 173, 6, 8.865 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 174, 6, 8.864 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 175, 6, 8.864 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 176, 6, 8.864 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 177, 6, 8.865 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 178, 6, 8.865 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 179, 6, 8.866 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 180, 6, 8.867 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 181, 6, 8.868 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 182, 6, 8.869 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 183, 6, 8.870 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 184, 6, 8.872 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 185, 6, 8.874 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 186, 6, 8.876 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 187, 6, 8.879 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 188, 6, 8.881 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 189, 6, 8.884 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 190, 6, 8.887 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 191, 6, 8.890 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 192, 6, 8.893 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 193, 6, 8.897 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 194, 6, 8.900 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 195, 6, 8.904 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 196, 6, 8.908 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 197, 6, 8.913 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 198, 6, 8.917 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 199, 6, 8.922 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 200, 6, 8.927 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 201, 6, 8.932 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 202, 6, 8.938 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 203, 6, 8.943 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 204, 6, 8.949 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 205, 6, 8.955 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 206, 6, 8.961 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 207, 6, 8.967 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 208, 6, 8.974 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 209, 6, 8.981 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 210, 6, 8.988 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 211, 6, 8.995 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 212, 6, 9.002 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 213, 6, 9.010 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 214, 6, 9.018 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 215, 6, 9.026 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 216, 6, 9.034 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 217, 6, 9.042 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 218, 6, 9.051 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 219, 6, 9.060 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 220, 6, 9.069 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 221, 6, 9.078 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 222, 6, 9.087 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 223, 6, 9.097 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 224, 6, 9.107 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 225, 6, 9.117 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 226, 6, 9.127 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 227, 6, 9.138 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 228, 6, 9.148 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 229, 6, 9.159 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 230, 6, 9.170 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 231, 6, 9.181 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 232, 6, 9.193 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 233, 6, 9.204 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 234, 6, 9.216 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 235, 6, 9.228 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 236, 6, 9.241 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 237, 6, 9.253 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 238, 6, 9.266 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 239, 6, 9.279 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 240, 6, 9.292 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 241, 6, 9.305 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 242, 6, 9.318 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 243, 6, 9.332 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 244, 6, 9.346 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 245, 6, 9.360 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 246, 6, 9.374 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 247, 6, 9.389 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 248, 6, 9.404 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 249, 6, 9.418 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 250, 6, 9.434 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 251, 6, 9.449 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 252, 6, 9.464 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 253, 6, 9.480 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 254, 6, 9.496 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 255, 6, 9.512 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 256, 6, 9.528 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 257, 6, 9.545 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 258, 6, 9.562 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 259, 6, 9.579 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 260, 6, 9.596 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 261, 6, 9.613 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 262, 6, 9.631 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 263, 6, 9.648 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 264, 6, 9.666 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 265, 6, 9.685 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 266, 6, 9.703 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 267, 6, 9.721 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 268, 6, 9.740 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 269, 6, 9.759 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 270, 6, 9.778 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 271, 6, 9.798 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 272, 6, 9.817 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 273, 6, 9.837 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 274, 6, 9.857 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 275, 6, 9.877 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 276, 6, 9.898 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 277, 6, 9.918 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 278, 6, 9.939 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 279, 6, 9.960 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 280, 6, 9.981 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 281, 6, 10.003 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 282, 6, 10.024 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 283, 6, 10.046 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 284, 6, 10.068 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 285, 6, 10.090 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 286, 6, 10.113 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 287, 6, 10.135 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 288, 6, 10.158 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 289, 6, 10.181 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 290, 6, 10.204 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 291, 6, 10.228 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 292, 6, 10.251 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 293, 6, 10.275 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 294, 6, 10.299 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 295, 6, 10.323 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 296, 6, 10.348 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 297, 6, 10.373 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 298, 6, 10.397 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 299, 6, 10.422 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 300, 6, 10.448 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 301, 6, 10.473 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 302, 6, 10.499 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 303, 6, 10.525 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 304, 6, 10.551 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 305, 6, 10.577 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 306, 6, 10.604 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 307, 6, 10.630 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 308, 6, 10.657 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 309, 6, 10.684 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 310, 6, 10.711 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 311, 6, 10.739 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 312, 6, 10.767 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 313, 6, 10.795 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 314, 6, 10.823 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 315, 6, 10.851 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 316, 6, 10.879 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 317, 6, 10.908 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 318, 6, 10.937 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 319, 6, 10.966 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 320, 6, 10.996 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 321, 6, 11.025 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 322, 6, 11.055 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 323, 6, 11.085 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 324, 6, 11.115 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 325, 6, 11.145 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 326, 6, 11.176 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 327, 6, 11.206 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 328, 6, 11.237 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 329, 6, 11.269 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 330, 6, 11.300 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 331, 6, 11.331 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 332, 6, 11.363 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 333, 6, 11.395 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 334, 6, 11.427 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 335, 6, 11.460 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 336, 6, 11.492 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 337, 6, 11.525 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 338, 6, 11.558 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 339, 6, 11.591 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 340, 6, 11.625 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 341, 6, 11.658 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 342, 6, 11.692 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 343, 6, 11.726 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 344, 6, 11.760 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 345, 6, 11.795 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 346, 6, 11.829 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 347, 6, 11.864 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 348, 6, 11.899 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 349, 6, 11.934 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 350, 6, 11.970 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 351, 6, 12.005 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 352, 6, 12.041 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 353, 6, 12.077 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 354, 6, 12.113 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 355, 6, 12.150 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 356, 6, 12.186 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 357, 6, 12.223 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 358, 6, 12.260 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 359, 6, 12.297 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 360, 6, 12.335 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 361, 6, 12.372 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 362, 6, 12.410 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 363, 6, 12.448 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 364, 6, 12.487 );
INSERT INTO model (doy, plan_id, energy) VALUES ( 365, 6, 12.525 );

GRANT SELECT ON TABLE model TO grafana;

create or replace function get_electricity_trend(
	out energy_percent float,
	out bill_percent float)
language plpgsql
as
$$
declare
begin
	drop table if exists temp_trend;
	
	-- initialize variables
	energy_percent := 0;
	bill_percent := 0;
	
	-- calculate model energy, bill up to yesterday
    CREATE TEMP TABLE temp_trend
	AS
	SELECT
	  date_trunc('year', now()) AS time, 
	  sum(energy) as energy, 
	  sum(energy * price + rate / 365.0) as bill
	FROM model JOIN plan ON model.plan_id = plan.id
	where
	  doy < EXTRACT('doy' FROM now() - INTERVAL '1 day');

	-- calculate energy and bill for current year up to yesterday
	insert into temp_trend (time, energy, bill)
	select
      date_trunc('year', now()) AS time,
      sum(energy) as energy,
      sum(bill) as bill
    from daily_view
    where
	  time between date_trunc('year', now()) AND now() - INTERVAL '1 day';
	
	-- calculate percentages
    select (lead(energy) over (order by time) - energy) / nullif(energy, 0) * 100 from temp_trend into energy_percent;
	  select (lead(bill) over (order by time) - bill) / nullif(bill, 0) * 100 from temp_trend into bill_percent;
end;
$$



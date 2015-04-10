#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

#include <ctime>

#include "vector2.h"
#include "vector3.h"

using namespace std;

bool optimizeFileSize = true;

Vector3* vertices;
Vector3* normals;
Vector2* uvs;

char* mtlib;

vector<Vector3> optimizedVertices;
vector<Vector3> optimizedNormals;
vector<Vector2> optimizedUvs;
vector<int> indices;

int vertexCount;
int quadCount;

int*** grid;
Vector3 minimum, maximum;
int nx, ny, nz;

bool LoadObj(char* filenameWithExt);
bool ExportObj(char* filenameWithExt);
void Optimize();
void ProcessSlice(int** slice, int xMax, int yMax, int index, Vector3 normal);
void EmitQuad(int sx, int sy, int ex, int ey, int index, Vector3 normal, Vector2 uv);
void InitGrid();
void ResetGrid();
void DeleteGrid();
void Clear();

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		cout << "No file is specified" << endl;
		return 0;
	}

	for(int i = 1; i < argc; i++)
	{
		char drive[512];
		char dir[512];
		char filename[512];
		char ext[512];

		_splitpath(argv[i], drive, dir, filename, ext);

		if(strcmp(ext, ".obj") !=0)
		{
			cout << "Please provide an .obj file." << endl;

			continue;
		}

		char filenameWithExt[512];
		char newFilename[512];
		char vers[16];
		strcpy(filenameWithExt, filename);
		strcat(filenameWithExt, "_optimized");
		strcat(filenameWithExt, ext);
		strcpy(newFilename, filenameWithExt);

		std::ifstream infile(filenameWithExt);
		bool isFilenameOk = !infile.good();
		infile.close();

		int versionNumber = 2;
		while(!isFilenameOk)
		{
			strcpy(newFilename, filename);
			itoa(versionNumber++, vers, 10);
			strcat(newFilename, "_optimized_");
			strcat(newFilename, vers);
			strcat(newFilename, ext);
			std::ifstream infile(newFilename);

			isFilenameOk = !infile.good();
			infile.close();
		}

		//cout << filenameWithExt << endl;
		//cout << argv[i] << endl;

		LoadObj(argv[i]);
		Optimize();
		ExportObj(newFilename);
		Clear();
	}



	return 0;
}

bool LoadObj(char* filenameWithExt)
{
	char *output = new char[512];
	char *token = new char[512];

	ifstream myReadFile;

	vector<Vector3> v,n, tVertices, tNormals;
	vector<Vector2> uv, tUvs;

	int d[12];

	char *nextToken;

	myReadFile.open(filenameWithExt);

	if (myReadFile.is_open()) 
	{
		while (!myReadFile.eof()) 
		{
			myReadFile.getline(output, 512);
			
			token = strtok_s( output, " ", &nextToken);

			if(token)
			{
				if(strcmp(token,"mtllib")==0)
				{
					token = strtok_s( 0, " ", &nextToken);

					mtlib = new char[512];
					strcpy(mtlib, token);
				}
				else if(strcmp(token,"v")==0)
				{
					Vector3 pos;
					sscanf_s(output + 2, "%f %f %f", &pos.x, &pos.y, &pos.z);
					v.push_back(pos);
				}
				else if(strcmp(token,"vn")==0)
				{
					Vector3 nor;
					sscanf_s(output + 3, "%f %f %f", &nor.x, &nor.y, &nor.z);
					n.push_back(nor);
				}
				else if(strcmp(token,"vt")==0)
				{
					Vector2 uvTemp;
					sscanf_s(output + 3, "%f %f", &uvTemp.x, &uvTemp.y);

					uv.push_back(uvTemp);
				}
				else if(strcmp(token,"f")==0)
				{
					sscanf_s(output + 2, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &d[0], &d[1], &d[2], &d[3], &d[4], &d[5], &d[6], &d[7], &d[8], &d[9], &d[10], &d[11]);

					tVertices.push_back(v.at(d[0] - 1));
					tVertices.push_back(v.at(d[3] - 1));
					tVertices.push_back(v.at(d[6] - 1));
					tVertices.push_back(v.at(d[9] - 1));

					tUvs.push_back(uv.at(d[1] - 1));
					tUvs.push_back(uv.at(d[4] - 1));
					tUvs.push_back(uv.at(d[7] - 1));
					tUvs.push_back(uv.at(d[10] - 1));

					tNormals.push_back(n.at(d[2] - 1));
					tNormals.push_back(n.at(d[5] - 1));
					tNormals.push_back(n.at(d[8] - 1));
					tNormals.push_back(n.at(d[11] - 1));
				}
			}
		}
	}
	
	vertices = new Vector3[tVertices.size()];
	normals = new Vector3[tVertices.size()];
	uvs = new Vector2[tVertices.size()];

	for(unsigned int i = 0; i < tVertices.size(); i++)
	{
		vertices[i] = tVertices.at(i);
		normals[i] = tNormals.at(i);
		uvs[i] = tUvs.at(i);
	}
	
	vertexCount = tVertices.size();
	quadCount = vertexCount / 4;

	myReadFile.close();

	delete[] output;
	delete[] token;
	//delete[] nextToken;

	//cout << "Vertex count: " << vertexCount << endl;
	//cout << "Quad count: " << quadCount << endl;

	return false;
}

bool ExportObj(char* filenameWithExt)
{
	DeleteGrid();
	
	nx++;
	ny++;
	nz++;

	

	grid = new int**[nx];

	for(int i = 0; i < nx; i++)
	{
		grid[i] = new int*[ny];

		for(int j = 0; j < ny; j++)
		{
			grid[i][j] = new int[nz];

			for(int k = 0; k < nz; k++)
			{
				grid[i][j][k] = -1;
			}
		}
	}



	vector<Vector2> uniqueUvs;
	vector<Vector3> uniqueVertices;

	// uniqueUvs = optimizedUvs;

	if(optimizeFileSize)
	{
		for(int i = 0; i < optimizedVertices.size(); i++)
		{
			Vector3 v = optimizedVertices[i];
			int x = v.x - minimum.x + 0.5;
			int y = v.y - minimum.y + 0.5;
			int z = v.z - minimum.z + 0.5;

			grid[x][y][z] = 1;
		}

		int counter = 0;

		for(int i = 0; i < nx; i++)
		{
			for(int j = 0; j < ny; j++)
			{
				for(int k = 0; k < nz; k++)
				{
					if(grid[i][j][k] == 1)
					{
						grid[i][j][k] = counter++;
						Vector3 v = minimum + Vector3(i,j,k);
						uniqueVertices.push_back(v);
					}
				}
			}
		}

		for(int i = 0; i < optimizedVertices.size(); i++)
		{
			Vector3 v = optimizedVertices[i];
			int x = v.x - minimum.x + 0.5;
			int y = v.y - minimum.y + 0.5;
			int z = v.z - minimum.z + 0.5;
			indices[i+(i/4)*2] = grid[x][y][z];
		}

		DeleteGrid();

		int* colors = new int[256];

		for(int i = 0; i < 256; i++)
			colors[i] = -1;

		for(int i = 0; i < optimizedUvs.size(); i++)
			colors[int(optimizedUvs[i].x*256)] = 1; 

		counter = 0;

		for(int i = 0; i < 256; i++)
		{
			if(colors[i] == 1)
			{
				colors[i] = counter++;
				uniqueUvs.push_back(Vector2((i + 0.5) / 256.0, 0.5));
			}
		}

		for(int i = 0; i < optimizedUvs.size(); i++)
			indices[6*i + 4] = colors[int(optimizedUvs[i].x * 256)];

		delete[] colors;
	}
	else
	{
		uniqueUvs = optimizedUvs;
		uniqueVertices = optimizedVertices;
	}

	optimizedNormals.push_back(Vector3::left);
	optimizedNormals.push_back(Vector3::up);
	optimizedNormals.push_back(Vector3::forward);

	optimizedNormals.push_back(Vector3::right);
	optimizedNormals.push_back(Vector3::down);
	optimizedNormals.push_back(Vector3::back);

	ofstream file;
	file.open (filenameWithExt);




	file << "# MagicaVoxel @ ephtracy" << endl << endl;

	file << "# material" << endl;
	file << "mtllib " << mtlib << endl;
	file << endl;

	file << "# normals" << endl;

	for(int i = 0; i < optimizedNormals.size(); i++)
	{
		Vector3 n = optimizedNormals[i];
		file << "vn " << n.x << " " << n.y << " " << n.z << endl;
	}

	file << endl;

	file << "# texcoords" << endl;

	for(int i = 0; i < uniqueUvs.size(); i++)
	{
		Vector2 uv = uniqueUvs[i];
		file << "vt " << uv.x << " " << uv.y << endl;
	}

	file << endl;

	file << "# verts" << endl;

	for(int i = 0; i < uniqueVertices.size(); i++)
	{
		Vector3 v = uniqueVertices[i];
		file << "v " << v.x << " " << v.y << " " << v.z << endl;
	}

	file << endl;

	file << "# faces" << endl;

	file << endl;

	file << "usemtl palette" << endl;

	file << endl;

	for(int i = 0; i < indices.size(); i += 6)
	{
		file << "f " << indices[i + 0] + 1 << "/" << indices[i + 4] + 1 << "/" << indices[i + 5] + 1 << " ";
		file <<			indices[i + 1] + 1 << "/" << indices[i + 4] + 1 << "/" << indices[i + 5] + 1 << " ";
		file <<			indices[i + 2] + 1 << "/" << indices[i + 4] + 1 << "/" << indices[i + 5] + 1 << " ";
		file <<			indices[i + 3] + 1 << "/" << indices[i + 4] + 1 << "/" << indices[i + 5] + 1 << endl;
	}

	file.close();

	cout << "File Exported. " << endl;

	return true;
}

void Optimize()
{
	InitGrid();

	int count = 0;

	for(int i = 0; i < quadCount; i++)
	{
		if(normals[4*i].x > 0.5)
		{
			count++;
			Vector3 a = vertices[4*i];
			Vector3 b = vertices[4*i + 1];
			Vector3 c = vertices[4*i + 2];
			Vector3 d = vertices[4*i + 3];

			Vector3 avg = (a + b + c + d) / 4;

			int x = (avg.x - minimum.x - 0.5);
			int y = (avg.y - minimum.y);
			int z = (avg.z - minimum.z);

			grid[x][y][z] = uvs[4*i].x * 256;
		}
	}

	int** slice = new int*[ny];

	for(int i = 0 ; i < ny; i++)
	{
		slice[i] = new int[nz];

		for(int j = 0 ; j < nz; j++)
		{
			slice[i][j] = -1;
		}
	}


	for(int i = 0; i < nx; i++)
	{
		for(int j = 0; j < ny; j++)
		{
			for(int k = 0; k < nz; k++)
			{
				slice[j][k] = grid[i][j][k];
			}
		}

		ProcessSlice(slice, ny, nz, i, Vector3::left);
	}

	for(int i = 0 ; i < ny; i++)
		delete[] slice[i];
	delete[] slice;

	ResetGrid();

	for(int i = 0; i < quadCount; i++)
	{
		if(normals[4*i].y > 0.5)
		{
			count++;
			Vector3 a = vertices[4*i];
			Vector3 b = vertices[4*i + 1];
			Vector3 c = vertices[4*i + 2];
			Vector3 d = vertices[4*i + 3];

			Vector3 avg = (a + b + c + d) / 4;

			int x = (avg.x - minimum.x);
			int y = (avg.y - minimum.y - 0.5);
			int z = (avg.z - minimum.z);

			grid[x][y][z] = uvs[4*i].x * 256;
		}
	}

	slice = new int*[nx];

	for(int i = 0 ; i < nx; i++)
	{
		slice[i] = new int[nz];

		for(int j = 0 ; j < nz; j++)
		{
			slice[i][j] = -1;
		}
	}

	for(int i = 0; i < ny; i++)
	{
		for(int j = 0; j < nx; j++)
		{
			for(int k = 0; k < nz; k++)
			{
				slice[j][k] = grid[j][i][k];
			}
		}

		ProcessSlice(slice, nx, nz, i, Vector3::up);
	}

	for(int i = 0 ; i < nx; i++)
		delete[] slice[i];
	delete[] slice;

	ResetGrid();

	for(int i = 0; i < quadCount; i++)
	{
		if(normals[4*i].z > 0.5)
		{
			count++;
			Vector3 a = vertices[4*i];
			Vector3 b = vertices[4*i + 1];
			Vector3 c = vertices[4*i + 2];
			Vector3 d = vertices[4*i + 3];

			Vector3 avg = (a + b + c + d) / 4;

			int x = (avg.x - minimum.x);
			int y = (avg.y - minimum.y);
			int z = (avg.z - minimum.z - 0.5);

			grid[x][y][z] = uvs[4*i].x * 256;
		}
	}

	slice = new int*[nx];

	for(int i = 0 ; i < nx; i++)
	{
		slice[i] = new int[ny];

		for(int j = 0 ; j < ny; j++)
		{
			slice[i][j] = -1;
		}
	}

	for(int i = 0; i < nz; i++)
	{
		for(int j = 0; j < nx; j++)
		{
			for(int k = 0; k < ny; k++)
			{
				slice[j][k] = grid[j][k][i];
			}
		}

		ProcessSlice(slice, nx, ny, i, Vector3::forward);
	}

	for(int i = 0 ; i < nx; i++)
		delete[] slice[i];
	delete[] slice;

	/////////////////
	/////////////////

	ResetGrid();

	for(int i = 0; i < quadCount; i++)
	{
		if(normals[4*i].x < -0.5)
		{
			count++;
			Vector3 a = vertices[4*i];
			Vector3 b = vertices[4*i + 1];
			Vector3 c = vertices[4*i + 2];
			Vector3 d = vertices[4*i + 3];

			Vector3 avg = (a + b + c + d) / 4;

			int x = (avg.x - minimum.x + 0.5);
			int y = (avg.y - minimum.y);
			int z = (avg.z - minimum.z);

			grid[x][y][z] = uvs[4*i].x * 256;
		}
	}

	slice = new int*[ny];

	for(int i = 0 ; i < ny; i++)
	{
		slice[i] = new int[nz];

		for(int j = 0 ; j < nz; j++)
		{
			slice[i][j] = -1;
		}
	}


	for(int i = 0; i < nx; i++)
	{
		for(int j = 0; j < ny; j++)
		{
			for(int k = 0; k < nz; k++)
			{
				slice[j][k] = grid[i][j][k];
			}
		}

		ProcessSlice(slice, ny, nz, i, Vector3::right);
	}

	for(int i = 0 ; i < ny; i++)
		delete[] slice[i];
	delete[] slice;

	ResetGrid();

	for(int i = 0; i < quadCount; i++)
	{
		if(normals[4*i].y < -0.5)
		{
			count++;
			Vector3 a = vertices[4*i];
			Vector3 b = vertices[4*i + 1];
			Vector3 c = vertices[4*i + 2];
			Vector3 d = vertices[4*i + 3];

			Vector3 avg = (a + b + c + d) / 4;

			int x = (avg.x - minimum.x);
			int y = (avg.y - minimum.y + 0.5);
			int z = (avg.z - minimum.z);

			grid[x][y][z] = uvs[4*i].x * 256;
		}
	}

	slice = new int*[nx];

	for(int i = 0 ; i < nx; i++)
	{
		slice[i] = new int[nz];

		for(int j = 0 ; j < nz; j++)
		{
			slice[i][j] = -1;
		}
	}

	for(int i = 0; i < ny; i++)
	{
		for(int j = 0; j < nx; j++)
		{
			for(int k = 0; k < nz; k++)
			{
				slice[j][k] = grid[j][i][k];
			}
		}

		ProcessSlice(slice, nx, nz, i, Vector3::down);
	}

	for(int i = 0 ; i < nx; i++)
		delete[] slice[i];
	delete[] slice;

	ResetGrid();

	for(int i = 0; i < quadCount; i++)
	{
		if(normals[4*i].z < -0.5)
		{
			count++;
			Vector3 a = vertices[4*i];
			Vector3 b = vertices[4*i + 1];
			Vector3 c = vertices[4*i + 2];
			Vector3 d = vertices[4*i + 3];

			Vector3 avg = (a + b + c + d) / 4;

			int x = (avg.x - minimum.x);
			int y = (avg.y - minimum.y);
			int z = (avg.z - minimum.z + 0.5);

			grid[x][y][z] = uvs[4*i].x * 256;
		}
	}

	slice = new int*[nx];

	for(int i = 0 ; i < nx; i++)
	{
		slice[i] = new int[ny];

		for(int j = 0 ; j < ny; j++)
		{
			slice[i][j] = -1;
		}
	}

	for(int i = 0; i < nz; i++)
	{
		for(int j = 0; j < nx; j++)
		{
			for(int k = 0; k < ny; k++)
			{
				slice[j][k] = grid[j][k][i];
			}
		}

		ProcessSlice(slice, nx, ny, i, Vector3::back);
	}

	for(int i = 0 ; i < nx; i++)
		delete[] slice[i];
	delete[] slice;
	//cout << count << endl;

	/*cout << "Min: " << minimum << endl;
	cout << "Max: " << maximum << endl;
	cout << nx << " " << ny << " " << nz << endl;*/
}

void ProcessSlice(int** slice, int xMax, int yMax, int index, Vector3 normal)
{
	//for(int i = 0; i < xMax; i++)
	//{
	//	for(int j = 0; j < yMax; j++)
	//	{
	//		cout << slice[i][j] << " ";
	//	}

	//	cout << endl;
	//}
	//cout << endl;

	for(int i = 0; i < xMax; i++)
	{
		for(int j = 0; j < yMax; j++)
		{
			int sx = i;
			int sy = j;

			int ex = i + 1;
			int ey = j + 1;

			bool isFinished = false;

			int value = slice[i][j];

			if(value == -1)
				continue;

			while(!isFinished)
			{
				// down
				bool shouldGrowDown = true;

				if(ey >= yMax)
					shouldGrowDown = false;
				else
				{
					for(int ii = sx; ii < ex; ii++)
					{
						if(slice[ii][ey] != value)
						{
							shouldGrowDown = false;
							break;
						}
					}
				}

				if(shouldGrowDown)
				{
					for(int ii = sx; ii < ex; ii++)
					{
						slice[ii][ey] = -1;
					}

					ey++;
				}

				bool shouldGrowRight = true;

				// right
				if(ex >= xMax)
					shouldGrowRight = false;
				else
				{
					for(int jj = sy; jj < ey; jj++)
					{
						if(slice[ex][jj] != value)
						{
							shouldGrowRight = false;
							break;
						}
					}
				}

				if(shouldGrowRight)
				{
					for(int jj = sy; jj < ey; jj++)
					{
						slice[ex][jj] = -1;
					}

					ex++;
				}

				if(!shouldGrowDown && !shouldGrowRight)
				{
					Vector2 uv((value + 0.5) / 256.0, 0.5);
					isFinished = true;
					EmitQuad(sx, sy, ex, ey, index, normal, uv);
				}
			}
		}
	}

	for(int i = 0; i < xMax; i++)
		for(int j = 0; j < yMax; j++)
			slice[i][j] = -1;
}

void EmitQuad(int sx, int sy, int ex, int ey, int index, Vector3 normal, Vector2 uv)
{
	Vector3 a, b, c, d;

	index++;

	int normalIndex = 0;

	if(normal.x > 0.5)
	{
		a = minimum + normal * index + Vector3::up * sx + Vector3::forward * sy;
		b = minimum + normal * index + Vector3::up * ex + Vector3::forward * sy;
		c = minimum + normal * index + Vector3::up * ex + Vector3::forward * ey;
		d = minimum + normal * index + Vector3::up * sx + Vector3::forward * ey;
		normalIndex = 0;
	}
	else if(normal.y > 0.5)
	{
		a = minimum + normal * index + Vector3::left * sx + Vector3::forward * sy;
		b = minimum + normal * index + Vector3::left * sx + Vector3::forward * ey;
		c = minimum + normal * index + Vector3::left * ex + Vector3::forward * ey;
		d = minimum + normal * index + Vector3::left * ex + Vector3::forward * sy;
		normalIndex = 1;
	}
	else if(normal.z > 0.5)
	{
		a = minimum + normal * index + Vector3::left * sx + Vector3::up * sy;
		b = minimum + normal * index + Vector3::left * ex + Vector3::up * sy;
		c = minimum + normal * index + Vector3::left * ex + Vector3::up * ey;
		d = minimum + normal * index + Vector3::left * sx + Vector3::up * ey;
		normalIndex = 2;
	}
	else if(normal.x < -0.5)
	{
		a = minimum + -normal * (index - 1) + Vector3::up * sx + Vector3::forward * sy;
		d = minimum + -normal * (index - 1) + Vector3::up * ex + Vector3::forward * sy;
		c = minimum + -normal * (index - 1) + Vector3::up * ex + Vector3::forward * ey;
		b = minimum + -normal * (index - 1) + Vector3::up * sx + Vector3::forward * ey;
		normalIndex = 3;
	}
	else if(normal.y < -0.5)
	{
		a = minimum + -normal * (index - 1) + Vector3::left * sx + Vector3::forward * sy;
		d = minimum + -normal * (index - 1) + Vector3::left * sx + Vector3::forward * ey;
		c = minimum + -normal * (index - 1) + Vector3::left * ex + Vector3::forward * ey;
		b = minimum + -normal * (index - 1) + Vector3::left * ex + Vector3::forward * sy;
		normalIndex = 4;
	}
	else if(normal.z < -0.5)
	{
		a = minimum + -normal * (index - 1) + Vector3::left * sx + Vector3::up * sy;
		d = minimum + -normal * (index - 1) + Vector3::left * ex + Vector3::up * sy;
		c = minimum + -normal * (index - 1) + Vector3::left * ex + Vector3::up * ey;
		b = minimum + -normal * (index - 1) + Vector3::left * sx + Vector3::up * ey;
		normalIndex = 5;
	}

	int vertexIndex = optimizedVertices.size();
	int uvIndex = optimizedUvs.size();
	
	optimizedUvs.push_back(uv);

	optimizedVertices.push_back(a);
	optimizedVertices.push_back(b);
	optimizedVertices.push_back(c);
	optimizedVertices.push_back(d);

	indices.push_back(vertexIndex + 0);
	indices.push_back(vertexIndex + 1);
	indices.push_back(vertexIndex + 2);
	indices.push_back(vertexIndex + 3);

	indices.push_back(uvIndex);

	indices.push_back(normalIndex);
}

void InitGrid()
{
	minimum = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	maximum = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	
	for(int i = 0; i < vertexCount; i++)
	{
		maximum.x = vertices[i].x < maximum.x ? maximum.x : vertices[i].x;
		maximum.y = vertices[i].y < maximum.y ? maximum.y : vertices[i].y;
		maximum.z = vertices[i].z < maximum.z ? maximum.z : vertices[i].z;

		minimum.x = vertices[i].x > minimum.x ? minimum.x : vertices[i].x;
		minimum.y = vertices[i].y > minimum.y ? minimum.y : vertices[i].y;
		minimum.z = vertices[i].z > minimum.z ? minimum.z : vertices[i].z;
	}

	Vector3 size = maximum - minimum;

	nx = size.x + 0.5f;
	ny = size.y + 0.5f;
	nz = size.z + 0.5f;

	grid = new int**[nx];

	for(int i = 0; i < nx; i++)
	{
		grid[i] = new int*[ny];

		for(int j = 0; j < ny; j++)
		{
			grid[i][j] = new int[nz];

			for(int k = 0; k < nz; k++)
			{
				grid[i][j][k] = -1;
			}
		}
	}
}

void ResetGrid()
{
	for(int i = 0; i < nx; i++)
	{
		for(int j = 0; j < ny; j++)
		{
			for(int k = 0; k < nz; k++)
			{
				grid[i][j][k] = -1;
			}
		}
	}
}

void DeleteGrid()
{
	for(int i = 0; i < nx; i++)
	{
		for(int j = 0; j < ny; j++)
		{
			delete[] grid[i][j];
		}

		delete[] grid[i];
	}

	delete[] grid;
}

void Clear()
{
	delete[] vertices;
	delete[] normals;
	delete[] uvs;

	delete[] mtlib;

	optimizedVertices.clear();
	optimizedNormals.clear();
	optimizedUvs.clear();
	indices.clear();
}
#include "pch.h"

#include <Kore/Graphics4/Graphics.h>
#include <Kore/Graphics4/Shader.h>
#include <Kore/IO/FileReader.h>
#include <Kore/System.h>

#include <limits>
#include <stdio.h>
#include <stdlib.h>

using namespace Kore;

namespace {
	Graphics4::Shader* vertexShader;
	Graphics4::Shader* fragmentShader;
	Graphics4::Program* program;
	Graphics4::VertexBuffer* vertices;
	Graphics4::IndexBuffer* indices;

	void update() {
		printf("update\n");
		Graphics4::begin();
		Graphics4::clear(Kore::Graphics4::ClearColorFlag);

		program->set();
		Graphics4::setVertexBuffer(*vertices);
		Graphics4::setIndexBuffer(*indices);
		Graphics4::drawIndexedVertices();

		Graphics4::end();
		Graphics4::swapBuffers();
	}
}

int kore(int argc, char** argv) {
	Kore::System::setName("Shader");
	Kore::System::setup();
	Kore::WindowOptions options;
	options.title = "Shader";
	options.width = 1024;
	options.height = 768;
	options.x = 100;
	options.y = 100;
	options.targetDisplay = -1;
	options.mode = WindowModeWindow;
	options.rendererOptions.depthBufferBits = 16;
	options.rendererOptions.stencilBufferBits = 8;
	options.rendererOptions.textureFormat = 0;
	options.rendererOptions.antialiasing = 0;
	Kore::System::initWindow(options);
	Kore::System::setCallback(update);

	FileReader vs("shader.vert");
	FileReader fs("shader.frag");
	vertexShader = new Graphics4::Shader(vs.readAll(), vs.size(), VertexShader);
	fragmentShader = new Graphics4::Shader(fs.readAll(), fs.size(), FragmentShader);
	Graphics4::VertexStructure structure;
	structure.add("pos", Float3VertexData);
	program = new Graphics4::Program;
	program->setVertexShader(vertexShader);
	program->setFragmentShader(fragmentShader);
	program->link(structure);

	vertices = new Graphics4::VertexBuffer(3, structure);
	float* v = vertices->lock();
	v[0] = -1;
	v[1] = -1;
	v[2] = 0.5;
	v[3] = 1;
	v[4] = -1;
	v[5] = 0.5;
	v[6] = -1;
	v[7] = 1;
	v[8] = 0.5;
	vertices->unlock();

	indices = new Graphics4::IndexBuffer(3);
	int* i = indices->lock();
	i[0] = 0;
	i[1] = 1;
	i[2] = 2;
	indices->unlock();

	Kore::System::start();

	return 0;
}

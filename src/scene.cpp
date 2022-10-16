#include"scene.h"
#include"geometry.h"
#include"axis.h"
#include<algorithm>
#include<GL/glew.h>
#include<iostream>
#include<map>
#include<utility>

/**
 * @brief Construtor para a classe scene
 * 
 */
scene::scene()
{
   this->theAxis = new axis;
   this->theAxis->setColor(0.0f, 1.0f, 0.0f);
   this->nextID = 1;
}


/**
 * @brief Destrutor para a classe scene
 * 
 */
scene::~scene()
{
    delete this->theAxis;
}

/**
 * @brief Método de desenho para a cena - Desenha todos os objetos na cena
 * 
 */
void scene::draw()
{
    this->theAxis->draw();
    
    for(std::map<unsigned int,solid*>::iterator it = this->objects.begin(); it != this->objects.end(); it++)
    {
        it->second->draw();
    }
}

/**
 * @brief Adiciona um objeto a cena
 * 
 * @param object : Ponteiro para o objeto a ser desenhado
 * @return unsigned int : ID != 0 atribuído ao objeto caso a inserção tenha sido bem-sucedida 
 * @return 0 : caso a inserção tenha sido mal-sucedida
 */
unsigned int scene::addObject(solid* object)
{
	/*if(this->checkCollision(object).second)
	{
		return 0;
	}else*/{
	
		this->objects[nextID] = object;
		nextID++;
		return nextID-1;
	}
}

/**
 * @brief Remove um objeto da cena
 * 
 * @param id : ID do objeto a ser removida
 * @return solid* : ponteiro para o objeto removido caso a remoção tenha sido bem-sucedida
 * @return nullptr : caso a remoção tenha sido mal-sucedida
 */
solid* scene::removeObject(unsigned int id)
{
    std::map<unsigned int, solid*>::iterator it = this->objects.find(id);
    solid* temp = nullptr;
    
    if(it != this->objects.end())
    {
		temp = it->second;
		this->objects.erase(it);
    }
    return temp;
}


/**
 * @brief Getter para um objeto da cena
 * 
 * @param id : ID do objeto a ser recuperado
 * 
 * @return std::pair<unsigned int, solid*> {ID, solid*} - caso o getter tenha sido bem-sucedido
 * @return std::pair<unsigned int, solid*> {0, nullptr} - caso o getter tenha sido mal-sucedido
 */
std::pair<unsigned int, solid*> scene::getObject(unsigned int id)
{
    std::map<unsigned int, solid*>::iterator it = this->objects.find(id);
    
    std::pair<unsigned int, solid*> object{0,nullptr};    
    
    if(it != this->objects.end())
    {
		object.first = it->first;
		object.second = it->second;
    }
    
    return object;
}


/**
 * @brief Checa colisão entre um ponto com coordenadas (x,y) e um objeto da cena
 * 
 * @param x : coordenada x do ponto
 * @param y : coordenada y do ponto
 * @return std::pair<unsigned int, solid*> {ID, solid*} caso haja colisão
 * @return std::pair<unsigned int, solid*> {0, nullptr} - caso não haja colisão
 */
/*std::pair<unsigned int, solid*> scene::checkCollision(float x, float y)
{
	
	std::pair<unsigned int, solid*>object = {0, nullptr};
	for(std::map<unsigned int,solid*>::iterator it = this->objects.begin(); it != this->objects.end(); it++)
    {
        if(it->second->collision(x,y,0.0f))
        {
			object.first = it->first;
			object.second = it->second;
			break;
        }
    }
    
    return object;
}*/


/**
 * @brief Checa colisão entre um objeto qualquer e um objeto da cena
 * 
 * @param geo : objeto com o qual deseja-se verificar a colisão de um elemento da cena
 * @return true : caso haja colisão com o objeto passado por argumento
 * @return false : caso haja colisão com o objeto passado pr argumento
 */
bool scene::checkCollision(solid* geo)
{
	
	for(std::map<unsigned int,solid*>::iterator it = this->objects.begin(); it != this->objects.end(); it++)
    {
        if(it->second->collision(geo))
			return true;
    }
    return false;
}
/**
 * @brief Iterator para o início da cena
 * 
 * @return sceneIterator : iterator para o início da cena
 */
sceneIterator scene::begin()
{
    return this->objects.begin();
}

/**
 * @brief Iterator para o fim da cena
 * 
 * @return sceneIterator : iterator para o fim da cena
 */
sceneIterator scene::end()
{
    return this->objects.end();
}

/**
 * @brief Atualiza as matrizes view e projection dos objetos da cena.
 * 
 * @param view Matriz view.
 * @param projection Matrix de projeção.
 */
void scene::updateCam(const matrix &view, const matrix &projection)
{
    sceneIterator it = this->objects.begin();

    for(it; it!=this->objects.end(); it++)
    {
        (*it).second->setProjection(projection);
        (*it).second->setView(view);
    }

    this->theAxis->setProjection(projection);
    this->theAxis->setView(view);

}


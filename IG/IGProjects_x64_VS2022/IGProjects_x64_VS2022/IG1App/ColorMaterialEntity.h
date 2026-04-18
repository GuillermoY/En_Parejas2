#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ColorMaterialEntity : public SingleColorEntity
{
private:
public:
	ColorMaterialEntity();
	//virtual void render(const glm::mat4& modelViewMat) const override; // Sobreescribimos render 
};
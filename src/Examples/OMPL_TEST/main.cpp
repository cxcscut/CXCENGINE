#include <iostream>

#include "../FBXSDK-Test/RobotHand.h"
#include "../FBXSDK-Test/Kinematics.h"

using namespace std;

#define MOVING_STEP 20.0f
#define MOVING_ORIENTATION_STEP (glm::radians(45.0f))

using namespace cxc;

#ifdef WIN32

static const std::string hand_file = "C:\\Users\\39317\\Desktop\\git\\cxcengine\\src\\Examples\\RobotSim\\Model\\SZrobotl.obj";
static const std::string plane_file = "C:\\Users\\39317\\Desktop\\git\\cxcengine\\src\\Examples\\RobotSim\\Model\\plane.obj";
static const std::string table_file = "C:\\Users\\39317\\Desktop\\git\\cxcengine\\src\\Examples\\RobotSim\\Model\\table.obj";

static const std::string VertexShaderPath = "C:\\Users\\39317\\Desktop\\git\\cxcengine\\src\\Engine\\Shader\\StandardVertexShader.glsl";
static const std::string FragmentShaderPath = "C:\\Users\\39317\\Desktop\\git\\cxcengine\\src\\Engine\\Shader\\StandardFragmentShader.glsl";

#else

static const std::string hand_file = "../RobotSim/Model/SZrobotl.obj";
static const std::string plane_file = "../RobotSim/Model/plane.obj";
static const std::string table_file = "../RobotSim/Model/table.obj";

static const std::string VertexShaderPath = "../../Engine/Shader/StandardVertexShader.glsl";
static const std::string FragmentShaderPath = "../../Engine/Shader/StandardFragmentShader.glsl";

#endif // WIN32

std::shared_ptr<Robothand> m_LeftPtr, m_RightPtr;
std::shared_ptr<Object3D> Table, Plane;

auto keycallback = [=](int key, int scancode, int action, int mods) {
	auto pEngine = EngineFacade::GetInstance();

	// W pressed
	if (glfwGetKey(pEngine->m_pWindowMgr->GetWindowHandle(), GLFW_KEY_W) == GLFW_PRESS)
	{
		if (m_LeftPtr)
		{
			m_LeftPtr->MovingArmOffset(glm::vec3({MOVING_STEP,0,0}));
		}
	}

	// S pressed
	if (glfwGetKey(pEngine->m_pWindowMgr->GetWindowHandle(), GLFW_KEY_S) == GLFW_PRESS)
	{
		if (m_LeftPtr)
		{
			m_LeftPtr->MovingArmOffset(glm::vec3({ -MOVING_STEP,0,0 }));
		}
	}

	// A pressed
	if (glfwGetKey(pEngine->m_pWindowMgr->GetWindowHandle(), GLFW_KEY_A) == GLFW_PRESS)
	{
		if (m_LeftPtr)
		{
			m_LeftPtr->MovingArmOffset(glm::vec3({0, MOVING_STEP,0 }));
		}
	}

	// D pressed
	if (glfwGetKey(pEngine->m_pWindowMgr->GetWindowHandle(), GLFW_KEY_D) == GLFW_PRESS)
	{
		if (m_LeftPtr)
		{
			m_LeftPtr->MovingArmOffset(glm::vec3({ 0, -MOVING_STEP,0 }));
		}
	}

	// PageUp pressed
	if (glfwGetKey(pEngine->m_pWindowMgr->GetWindowHandle(), GLFW_KEY_PAGE_UP) == GLFW_PRESS)
	{
		if (m_LeftPtr)
		{
			m_LeftPtr->MovingArmOffset(glm::vec3({ 0, 0,MOVING_STEP }));
		}
	}

	// PageDown pressed
	if (glfwGetKey(pEngine->m_pWindowMgr->GetWindowHandle(), GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
	{
		if (m_LeftPtr)
		{
			m_LeftPtr->MovingArmOffset(glm::vec3({ 0, 0,-MOVING_STEP }));
		}
	}

	if (glfwGetKey(pEngine->m_pWindowMgr->GetWindowHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		pEngine->GameOver = true;
	}
};

int main()
{
	// Accquire engine pointer
	auto pEngine = EngineFacade::GetInstance();

	// Engine configuration
	{

		pEngine->SetGravity(0, -0.1, 0);

		pEngine->m_pWindowMgr->InitGL();

		pEngine->m_pWindowMgr->SetWindowHeight(600);
		pEngine->m_pWindowMgr->SetWindowWidth(800);

		pEngine->SetVertexShaderPath(VertexShaderPath);
		pEngine->SetFragmentShaderPath(FragmentShaderPath);

		pEngine->m_pWindowMgr->SetWindowTitle("Engine test");
		pEngine->m_pWindowMgr->isDecoraded = true;

		pEngine->m_pSceneMgr->m_pCamera->eye_pos = glm::vec3(0, 2000, 2000);

		EngineFacade::KeyInputCallBack = keycallback;

		pEngine->InitWindowPosition(200,200);

		pEngine->MultiThreadingEnable();
	}

	pEngine->Init();

	// Object definition and loading
	{

		auto LoadRobothand = [&](int type) {
			if (type == ROBOTHAND_LEFT)
				m_LeftPtr = std::make_shared<Robothand>(type, hand_file);
			else
				m_RightPtr = std::make_shared<Robothand>(type, hand_file);
		};

		auto LoadPlane = [&]() {Plane = std::make_shared<Object3D>("plane", plane_file,"env"); };
		auto LoadTable = [&]() {
			Table = std::make_shared<Object3D>("table", table_file, "env");
		};

		std::thread left_hand(LoadRobothand, ROBOTHAND_LEFT);
		std::thread plane(LoadPlane);
		std::thread table(LoadTable);

		left_hand.join();
		plane.join();
		table.join();

		if (!Plane || !Plane->CheckLoaded()) return 0;
		if (!m_LeftPtr || !m_LeftPtr->CheckLoaded()) return 0;
		//if (!Table || !Table->CheckLoaded()) return 0;

		pEngine->addObject(Plane, true);
		pEngine->addObject(m_LeftPtr,true);
		pEngine->addObject(Table,true);

	}

	// Adding user code here
	{
		Plane->Translation(glm::vec3(0,-1,0));

		m_LeftPtr->InitOriginalDegrees();

		m_LeftPtr->RotateJoint("arm_left2", 90.0f);

		// Set initial pose of robot arm
		m_LeftPtr->SetBaseDegrees({ 0,0,90,0,-90,0 });

		//m_LeftPtr->MovingArm(0,-200,500,0,0,0);
	}

	// Start engine
	pEngine->run();

	// Waiting for stop when condition satisifies
	pEngine->waitForStop();

    return 0;
}

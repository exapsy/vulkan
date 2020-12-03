#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <ranges>

const u_int32_t WIDTH = 800;
const u_int32_t HEIGHT = 600;

class Triangle {
	public:
		void run() {
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}
	private:
		GLFWwindow* window;
		VkInstance instance;
		void initWindow() {
			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			window = glfwCreateWindow(WIDTH, HEIGHT, "vulkan", nullptr, nullptr);
		}
		void initVulkan() {
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hello Triangle";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "No Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_2;

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

      checkExtensionSupport();

			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			createInfo.enabledExtensionCount = glfwExtensionCount;
			createInfo.ppEnabledExtensionNames = glfwExtensions;
			createInfo.enabledLayerCount = 0;

			VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

			if (result != VK_SUCCESS) {
				throw std::runtime_error("failed to create instance");
			}

			std::vector<VkExtensionProperties> extensions = getAvailableExtensions();

			std::cout << "available extensions:\n";

			for (const auto& extension : extensions) {
				std::cout << "\t" << extension.extensionName << "\tv" << extension.specVersion << "\n";
			}
		}
		void checkExtensionSupport() {
			uint32_t glfwExtensionCount = 0;
			const char** requiredExtensions;
			requiredExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
			
			std::vector<VkExtensionProperties> availableExtensions
        = getAvailableExtensions();

      for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        const char* requiredExtension = requiredExtensions[i];

        const bool isExtensionAvailable = std::find_if(
              availableExtensions.begin(),
              availableExtensions.end(),
              [=](VkExtensionProperties& availableExtension) {
                return strcmp(
                    availableExtension.extensionName,
                    requiredExtension
                );
              }
          ) != availableExtensions.end();
        if (!isExtensionAvailable) {
          std::cerr << "Extension " << requiredExtension << " is not available\n";
          exit(EXIT_FAILURE);
        }
      }
		}
		std::vector<VkExtensionProperties> getAvailableExtensions() {
			uint32_t extensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> extensions(extensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
			return extensions;
		}
		void mainLoop() {
			while (!glfwWindowShouldClose(window)) {
				glfwPollEvents();
			}
		}
		void cleanup() {
			glfwDestroyWindow(window);
			glfwTerminate();
		}
};

int main() {
	Triangle app;

	try {
		app.run();
	} catch (std::exception e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

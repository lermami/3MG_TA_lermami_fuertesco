/**
 * @file defaul_components.hpp
 * @brief Header file for the components.
 *
 * @defgroup defaul_components defaul_components
 * @brief This file defines the defaul components of entities.
 */
#pragma once

#include "buffer.hpp"
#include "vector_2.hpp"
#include "vector_3.hpp"
#include "vector_4.hpp"
#include "matrix_4.hpp"
#include "sound/soundsource.h"
#include "light.hpp"
#include "Window.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

/**
 * @brief Abstract base class for components attached to entities.
 *
 * Defines virtual functions for adding, removing, and getting the number of components attached to an entity.
 */
struct component_base {
	virtual void add_component() = 0;
	virtual void add_component(int position) = 0;
	virtual void delete_component(size_t id) = 0;
	virtual size_t size() = 0;
};

/**
 * @tparam T The type of component to store in the list.
 * @brief Template class that manages a list of components of type T.
 *
 * Inherits from component_base and provides functionality for adding, removing, and accessing components.
 */
template<typename T>
struct component_list : component_base {
	std::vector < std::optional<T>> components_; /**< List of components of type T. */

	/**
 * @brief Adds a new empty component of type T to the list.
 *
 * This function creates a new `std::optional<T>` object with a null value and adds it to the end of the `components_` list.
 */
	virtual void add_component() override {
		std::optional<T> a;
		components_.emplace_back(a);
	}

	/**
 * @brief Adds a new component of type T at a specific position in the list.
 *
 * This function creates a new instance of type T and assigns it to the specified position (1-based indexing) in the `components_` list.
 *
 * @throws std::out_of_range If the specified position is out of bounds.
 *
 * @note The position argument is 1-based because entities typically start with ID 1. This function internally adjusts the index to 0-based for the `components_` vector.
 */
	virtual void add_component(int position) override {
		T a;
		//-1 because position refers to an entity that starts in 1
		components_[(size_t)position - 1] = a;
	}

	/**
	 * @brief Adds a component of type T at a specific position in the list.
	 *
	 * @param position The position where to add the component (1-based indexing).
	 * @param component The component to add.
	 */
	void add_component_at(size_t position, T& component) {
		assert(components_[position - 1] == std::nullopt);

		components_[position - 1] = component;

	}

	/**
 * @brief Gets the number of components currently stored in the list.
 *
 * This function returns the size of the `components_` vector, which represents the number of components attached to the entity.
 *
 * @return The number of components in the list.
 */
	virtual size_t size() override {
		return components_.size();
	}

	/**
 * @brief Removes a component from the list by its ID (0-based indexing).
 *
 * This function removes the component at the specified ID from the `components_` list. It asserts that the ID is not 0 (trying to remove the first element, which might be special) and then resets the corresponding `std::optional<T>` object to null, effectively removing the component.
 *
 * @throws std::out_of_range If the specified ID is out of bounds.
 *
 * @param id The ID of the component to remove.
 */
	virtual void delete_component(size_t id) override {
		assert(id != 0);
		components_[id - 1].reset();
		components_[id - 1] = std::nullopt;
	}
};

/**
 * @brief Struct representing a vertex in 3D space.
 *
 * A vertex typically represents a single point in 3D space and holds various attributes used for rendering.
 */
struct Vertex {
	/**
	 * @brief Position of the vertex in 3D space.
	 */
	Vec3 pos;

	/**
	 * @brief Normal vector of the vertex, used for lighting calculations.
	 */
	Vec3 normal;

	/**
	 * @brief UV coordinates for texture mapping.
	 */
	Vec2 uv;

	/**
	 * @brief Color of the vertex.
	 */
	Vec4 color;

	/**
	 * @brief Compares two vertices for equality.
	 *
	 * This operator compares the position, normal, and UV coordinates of two vertices for equality.
	 *
	 * @param o The other vertex to compare with.
	 *
	 * @return True if all components (position, normal, UV) are equal, false otherwise.
	 */
	bool operator==(const Vertex& o) const {
		return pos == o.pos && normal == o.normal && uv == o.uv;
	}
};

/**
 * @brief Struct representing an entity's transform data.
 *
 * This struct stores information about an entity's position, rotation, scale, and its model matrix.
 */
struct TransformComponent {
	/**
	 * @brief Model matrix of the entity, used for transforming the entity in 3D space.
	 */
	Mat4 model_matrix_;

	/**
	 * @brief Position of the entity in 3D space.
	 */
	Vec3 pos_;

	/**
	 * @brief Rotation of the entity, typically stored in Euler angles (degrees).
	 */
	Vec3 rot_;

	/**
	 * @brief Scale of the entity, used to modify its size.
	 */
	Vec3 size_;

	/**
	 * @brief Default constructor that initializes all members to zero.
	 */
	TransformComponent() = default;

	/**
	 * @brief Constructor that initializes the position, rotation, and scale of the entity.
	 *
	 * @param pos The initial position of the entity.
	 * @param rot The initial rotation of the entity.
	 * @param size The initial scale of the entity.
	 */
	TransformComponent(Vec3 pos, Vec3 rot = Vec3(0.0f, 0.0f, 0.0f), Vec3 size = Vec3(1.0f, 1.0f, 1.0f)) {
		pos_ = pos;
		rot_ = rot;
		size_ = size;
	}
};

/**
 * @brief Struct representing an entity's color information.
 *
 * This struct stores the RGBA color of an entity.
 */
struct ColorComponent {
	/**
	 * @brief RGBA color of the entity.
	 */
	Vec4 color_;

	/**
	 * @brief Default constructor that initializes the color to black (0, 0, 0, 1).
	 */
	ColorComponent() = default;

	/**
	 * @brief Constructor that initializes the color of the entity.
	 *
	 * @param color The RGBA color of the entity.
	 */
	ColorComponent(Vec4 color) {
		color_ = color;
	}
};

/**
 * @brief Struct representing an audio component attached to an entity.
 *
 * This struct manages a sound source object and allows attaching a sound buffer to it for playback.
 */
struct AudioComponent {
	/**
	 * @brief Internal sound source object used for audio playback.
	 */
	SoundSource sound_source_;

	/**
	 * @brief Constructor that creates an AudioComponent with a sound source and optionally plays it.
	 *
	 * This constructor takes various arguments to initialize the sound source:
	 *   @param buff Reference to a `SoundBuffer` object containing the audio data.
	 *   @param name A name for the sound source (for debugging purposes).
	 *   @param pos Position of the sound source in 3D space (ALfloat array of length 3).
	 *   @param speed Velocity of the sound source (ALfloat array of length 3).
	 *   @param gain Gain of the sound source (volume amplification). Defaults to 1.0f.
	 *   @param pitch Pitch of the sound source (alters playback speed). Defaults to 1.0f.
	 *   @param play If true, starts playing the sound after creation. Defaults to false.
	 *
	 * After setting up the sound source, it adds the provided sound buffer using the `addSound` function and optionally starts playback using the `Play` function.
	 */
	AudioComponent(SoundBuffer& buff, std::string name, ALfloat pos[3],
		ALfloat speed[3], float gain = 1.0f, float pitch = 1.0f, bool play = false)
		: sound_source_(name, pos, speed, gain, pitch) {
		sound_source_.addSound(&buff);
		if (play)
			sound_source_.Play();
	}

	/**
	 * @brief Default constructor that initializes an empty AudioComponent.
	 */
	AudioComponent() = default;

	/**
	 * @brief Copy assignment operator (default behavior is sufficient).
	 */
	AudioComponent& operator=(const AudioComponent& o) = default;
};

/**
 * @brief Struct representing a box collider attached to an entity.
 *
 * This struct defines a bounding box for collision detection purposes.
 */
struct BoxColliderComponent {
	/**
	 * @brief Half-extents of the box collider along each axis (X, Y, Z).
	 */
	Vec3 extent_;

	/**
	 * @brief Offset of the bounding box from the entity's center.
	 */
	Vec3 center_offset_;

	/**
	 * @brief Default constructor that initializes the box collider with zero extent and no offset.
	 */
	BoxColliderComponent() = default;

	/**
	 * @brief Constructor that initializes the extent and center offset of the box collider.
	 *
	 * @param extent Half-extents of the box collider along each axis (X, Y, Z).
	 * @param center_offset Offset of the bounding box from the entity's center. Defaults to (0, 0, 0).
	 */
	BoxColliderComponent(Vec3 extent, Vec3 center_offset = Vec3(0.0f, 0.0f, 0.0f)) {
		extent_ = extent;
		center_offset_ = center_offset;
	}
};

/**
 * @brief Struct representing geometric data for rendering.
 *
 * This struct holds a collection of vertices and indices used to define the mesh of a 3D object.
 */
struct Geometry {
	/**
	 * @brief List of vertices that define the geometry.
	 */
	std::vector<Vertex> vertex_;

	/**
	 * @brief List of indices that specify the order of vertices to form triangles.
	 */
	std::vector<unsigned> indices_;
};

/**
 * @brief Struct representing a component for rendering an entity.
 *
 * This struct stores information about the visual representation of an entity, including its name, buffers, program, and texture.
 */
struct RenderComponent {
	/**
	 * @brief Name of the render component.
	 */
	std::string name_;

	/**
	 * @brief Name of the vertex elements buffer used for rendering.
	 */
	std::string elements_buffer_;

	/**
	 * @brief Name of the vertex order buffer (indices) used for rendering.
	 */
	std::string order_buffer_;

	/**
	 * @brief ID of the shader program used for rendering this component.
	 */
	unsigned int program_;

	/**
	 * @brief ID of the texture used for rendering this component (optional).
	 */
	unsigned int texture_;

	/**
	 * @brief Default constructor that initializes all members with default values.
	 */
	RenderComponent() = default;

	/**
	 * @brief Constructor that initializes all members of the render component.
	 *
	 * @param name Name of the render component.
	 * @param elements Name of the vertex elements buffer.
	 * @param order Name of the vertex order buffer (indices).
	 * @param program ID of the shader program used for rendering.
	 * @param texture ID of the texture used for rendering (optional, can be -1).
	 */
	RenderComponent(const char* name, const char* elements, const char* order, unsigned program, unsigned texture) {
		name_ = name;
		elements_buffer_ = elements;
		order_buffer_ = order;
		program_ = program;
		texture_ = texture;
	}
};

/**
 * @brief Struct representing a camera component attached to an entity.
 *
 * This struct stores camera properties and provides methods for setting up projection and view matrices.
 */
struct CameraComponent {
  
	std::string name_;
  
	/**
	 * @brief Position of the camera in 3D space.
	 */
	Vec3 pos_;

	/**
	 * @brief Forward direction of the camera.
	 */
	Vec3 forward_;

	/**
	 * @brief Up direction of the camera (usually Y-axis).
	 */
	Vec3 up_;

	/**
	 * @brief Right direction of the camera (orthogonal to forward and up).
	 */
	Vec3 right_;

	/**
	 * @brief Movement speed of the camera.
	 */
	float speed_;

	/**
	 * @brief Mouse sensitivity for camera rotation.
	 */
	float sensitivity_;

	/**
	 * @brief Current projection mode used by the camera.
	 */
	ProjectionMode projectionMode_;

	float last_alpha;
	float last_omega;
  
	/**
	 * @brief Default constructor that initializes the camera with common values.
	 */
	CameraComponent() {
		right_ = Vec3(0.0f, 0.0f, 0.0f);
		up_ = Vec3(0.0f, 1.0f, 0.0f);
		forward_ = Vec3(0.0f, 0.0f, -1.0f);

		speed_ = 1.0f;
		sensitivity_ = 1.0f;

		projectionMode_ = ProjectionMode::kPerspective;

		last_alpha = -1.57f;
		last_omega = 0.0f;
	};

	CameraComponent(std::string name, float speed, float sensitivity, ProjectionMode mode = ProjectionMode::kPerspective) {
		name_ = name;
    
    right_ = Vec3(0.0f, 0.0f, 0.0f);
		up_ = Vec3(0.0f, 1.0f, 0.0f);
		forward_ = Vec3(0.0f, 0.0f, -1.0f);

		speed_ = speed;
		sensitivity_ = sensitivity;

		projectionMode_ = mode;
    
    last_alpha = -1.57f;
		last_omega = 0.0f;
	}
};

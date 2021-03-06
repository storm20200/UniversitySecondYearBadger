#include "Wheel.h"



// STL headers.
#include <exception>
#include <iostream>



#pragma region Constructors and destructor

Badger::Wheel::Wheel (Wheel&& move)
{
    *this = std::move (move);
}


Badger::Wheel& Badger::Wheel::operator= (Wheel&& move)
{
    if (this != &move)
    {
        // IActor.
        m_node = std::move (move.m_node);

        // Badger::IRotatableComponent.
        m_turnSpeed = std::move (move.m_turnSpeed);
        m_targetTurn = std::move (move.m_targetTurn);

        // Badger::Wheel.
        m_diameter = std::move (move.m_diameter);
        m_revolveModifier = std::move (move.m_revolveModifier);
    }

    return *this;
}


Badger::Wheel::~Wheel()
{
}

#pragma endregion


#pragma region Getters and setters

void Badger::Wheel::reset()
{
    // We don't reset the position or orientation because we just can't know how the wheels should be placed.
    m_node->setScale ({ 1.f, 1.f, 1.f });

    m_targetTurn = 0.f;
    m_node->yaw (Ogre::Radian (0.f));
}

#pragma endregion


#pragma region Actor functionality

bool Badger::Wheel::initialise (OgreApplication* const ogre, Ogre::SceneNode* root, const Ogre::String& name)
{
    try
    {
        // Pre-condition: We have a valid OgreApplication and SceneNode pointer.
        if (!ogre || !root)
        {
            throw std::invalid_argument ("Badger::Wheel::initialise(), required parameter 'ogre' or 'root' is a nullptr.");
        }

        // Initialise the entity.
        const auto entity = constructEntity (ogre, "Wheel.mesh", "blue");

        // Construct the node.
        m_node = constructNode (root, name, entity);

        // Reset the tracker.
        reset();
        
        // We're done, yay!
        return true;
    }

    catch (const std::exception& error)
    {
        std::cerr << "An exception was caught in Badger::Wheel::initialise(): " << error.what() << std::endl;
    }

    catch (...)
    {
        std::cerr << "An unknown error occurred in Badger::Wheel::initialise()." << std::endl;
    }
    
    return false;
}


void Badger::Wheel::updateSimulation (const float deltaTime)
{
    // Rotate the wheels sideways.
    rotateComponent (Ogre::Vector3::UNIT_Y, Ogre::Node::TS_PARENT, deltaTime);
}

#pragma endregion


#pragma region Simulation

void Badger::Wheel::revolve (const float distance)
{
    // Obtain the necessary constants for calculate the correct rotation.
    const float fullRevolution  { Ogre::Math::TWO_PI },
                pi              { Ogre::Math::PI };

    // Calculate how much to revolve the wheel by.
    const float circumference   { m_diameter  * pi },
                toRotate        { fullRevolution * (distance / circumference) * m_revolveModifier };

    // Rotate the wheel.
    const auto  rotation        = Ogre::Quaternion (Ogre::Radian (toRotate), Ogre::Vector3::UNIT_Y);
    
    m_node->rotate (rotation, Ogre::Node::TS_LOCAL);
}

#pragma endregion
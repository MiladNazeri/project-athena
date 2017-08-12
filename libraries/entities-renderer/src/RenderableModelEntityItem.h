//
//  RenderableModelEntityItem.h
//  interface/src/entities
//
//  Created by Brad Hefta-Gaub on 8/6/14.
//  Copyright 2014 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_RenderableModelEntityItem_h
#define hifi_RenderableModelEntityItem_h

#include <QString>
#include <QStringList>

#include <ModelEntityItem.h>

#include "RenderableEntityItem.h"

class Model;
class EntityTreeRenderer;

//#define MODEL_ENTITY_USE_FADE_EFFECT

class RenderableModelEntityItem : public ModelEntityItem, RenderableEntityInterface {
public:
    static EntityItemPointer factory(const EntityItemID& entityID, const EntityItemProperties& properties);

    RenderableModelEntityItem(const EntityItemID& entityItemID, bool dimensionsInitialized);

    virtual ~RenderableModelEntityItem();

    RenderableEntityInterface* getRenderableInterface() override { return this; }

    virtual void setDimensions(const glm::vec3& value) override;
    virtual void setModelURL(const QString& url) override;

    virtual EntityItemProperties getProperties(EntityPropertyFlags desiredProperties = EntityPropertyFlags()) const override;
    virtual bool setProperties(const EntityItemProperties& properties) override;
    virtual int readEntitySubclassDataFromBuffer(const unsigned char* data, int bytesLeftToRead,
                                                ReadBitstreamToTreeParams& args,
                                                EntityPropertyFlags& propertyFlags, bool overwriteLocalData,
                                                bool& somethingChanged) override;

    void doInitialModelSimulation();

    virtual bool addToScene(const EntityItemPointer& self, const render::ScenePointer& scene, render::Transaction& transaction) override;
    virtual void removeFromScene(const EntityItemPointer& self, const render::ScenePointer& scene, render::Transaction& transaction) override;


    void updateModelBounds();
    virtual void render(RenderArgs* args) override;
    virtual bool supportsDetailedRayIntersection() const override;
    virtual bool findDetailedRayIntersection(const glm::vec3& origin, const glm::vec3& direction,
                        bool& keepSearching, OctreeElementPointer& element, float& distance,
                        BoxFace& face, glm::vec3& surfaceNormal,
                        void** intersectedObject, bool precisionPicking) const override;
    ModelPointer getModel();
    ModelPointer getModelNotSafe();

    virtual bool needsToCallUpdate() const override;
    virtual void update(const quint64& now) override;

    virtual void setShapeType(ShapeType type) override;
    virtual void setCompoundShapeURL(const QString& url) override;

    virtual bool isReadyToComputeShape() override;
    virtual void computeShapeInfo(ShapeInfo& shapeInfo) override;

    void setCollisionShape(const btCollisionShape* shape) override;

    virtual bool contains(const glm::vec3& point) const override;

    virtual bool shouldBePhysical() const override;

    // these are in the frame of this object (model space)
    virtual glm::quat getAbsoluteJointRotationInObjectFrame(int index) const override;
    virtual glm::vec3 getAbsoluteJointTranslationInObjectFrame(int index) const override;
    virtual bool setAbsoluteJointRotationInObjectFrame(int index, const glm::quat& rotation) override;
    virtual bool setAbsoluteJointTranslationInObjectFrame(int index, const glm::vec3& translation) override;


    virtual glm::quat getLocalJointRotation(int index) const override;
    virtual glm::vec3 getLocalJointTranslation(int index) const override;
    virtual bool setLocalJointRotation(int index, const glm::quat& rotation) override;
    virtual bool setLocalJointTranslation(int index, const glm::vec3& translation) override;

    virtual void setJointRotations(const QVector<glm::quat>& rotations) override;
    virtual void setJointRotationsSet(const QVector<bool>& rotationsSet) override;
    virtual void setJointTranslations(const QVector<glm::vec3>& translations) override;
    virtual void setJointTranslationsSet(const QVector<bool>& translationsSet) override;

    virtual void loader() override;
    virtual void locationChanged(bool tellPhysics = true) override;

    virtual void resizeJointArrays(int newSize = -1) override;

    virtual int getJointIndex(const QString& name) const override;
    virtual QStringList getJointNames() const override;

    // These operate on a copy of the animationProperties, so they can be accessed
    // without having the entityTree lock.
    bool hasRenderAnimation() const { return !_renderAnimationProperties.getURL().isEmpty(); }
    const QString& getRenderAnimationURL() const { return _renderAnimationProperties.getURL(); }

    render::ItemID getMetaRenderItem() { return _myMetaItem; }

    // Transparency is handled in ModelMeshPartPayload
    bool isTransparent() override { return false; }

    void mapJoints(const QStringList& modelJointNames);
    bool jointsMapped() const {
        return _jointMappingURL == getAnimationURL() && _jointMappingCompleted;
    }

    AnimationPointer getAnimation() const {
        return _animation;
    }

    bool getMeshes(MeshProxyList& result) override;

private:
    QVariantMap parseTexturesToMap(QString textures);
    void remapTextures();

    void getCollisionGeometryResource();
    GeometryResource::Pointer _compoundShapeResource;
    ModelPointer _model = nullptr;
    bool _needsInitialSimulation = true;
    bool _needsModelReload = true;
    QSharedPointer<EntityTreeRenderer> _myRenderer;
    QString _lastTextures;
    QVariantMap _currentTextures;
    QVariantMap _originalTextures;
    bool _originalTexturesRead = false;
    bool _dimensionsInitialized = true;
#ifdef MODEL_ENTITY_USE_FADE_EFFECT
    bool _hasTransitioned{ false };
#endif
    AnimationPropertyGroup _renderAnimationProperties;

    render::ItemID _myMetaItem{ render::Item::INVALID_ITEM_ID };

    bool getAnimationFrame();

    bool _needsJointSimulation { false };
    bool _showCollisionGeometry { false };
    const void* _collisionMeshKey { nullptr };

    // used on client side
    bool _jointMappingCompleted { false };
    QVector<int> _jointMapping; // domain is index into model-joints, range is index into animation-joints
    QString _jointMappingURL;
    AnimationPointer _animation;
};

#endif // hifi_RenderableModelEntityItem_h

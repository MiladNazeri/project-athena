//
//  Created by Bradley Austin Davis on 2016/12/12
//  Copyright 2013-2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#pragma once
#ifndef hifi_TestScriptingInterface_h
#define hifi_TestScriptingInterface_h

#include <functional>
#include <QtCore/QObject>

class QScriptValue;

class TestScriptingInterface : public QObject {
    Q_OBJECT

public:
    void setTestResultsLocation(const QString path) { _testResultsLocation = path; }
    const QString& getTestResultsLocation() { return _testResultsLocation;  };

public slots:
    static TestScriptingInterface* getInstance();

    /**jsdoc
     * Exits the application
     * @function Test.quit
     */
    void quit();

    /**jsdoc
    * Waits for all texture transfers to be complete
    * @function Test.waitForTextureIdle
    */
    void waitForTextureIdle();

    /**jsdoc
    * Waits for all pending downloads to be complete
    * @function Test.waitForDownloadIdle
    */
    void waitForDownloadIdle();

    /**jsdoc
    * Waits for all file parsing operations to be complete
    * @function Test.waitForProcessingIdle
    */
    void waitForProcessingIdle();

    /**jsdoc
    * Waits for all pending downloads, parsing and texture transfers to be complete
    * @function Test.waitIdle
    */
    void waitIdle();

    /**jsdoc
    * Waits for establishment of connection to server
    * @function Test.waitForConnection
    */
    bool waitForConnection(qint64 maxWaitMs = 10000);

    /**jsdoc
    * Waits a specific number of milliseconds
    * @function Test.wait
    * @param {int} milliseconds - Number of milliseconds to wait
    */
    void wait(int milliseconds);

    /**jsdoc
    * Waits for all pending downloads, parsing and texture transfers to be complete
    * @function Test.loadTestScene
    * @param {string} sceneFile - URL of scene to load
    */
    bool loadTestScene(QString sceneFile);

    /**jsdoc
    * Clears all caches
    * @function Test.clear
    */
    void clear();

    /**jsdoc
    * Start recording Chrome compatible tracing events
    * logRules can be used to specify a set of logging category rules to limit what gets captured
    * @function Test.startTracing
    * @param {string} logrules [defaultValue=""] - See implementation for explanation
    */
    bool startTracing(QString logrules = "");

    /**jsdoc
    * Stop recording Chrome compatible tracing events and serialize recorded events to a file
    * Using a filename with a .gz extension will automatically compress the output file
    * @function Test.stopTracing
    * @param {string} filename - Name of file to save to
    * @returns {bool} True if successful.   
    */
    bool stopTracing(QString filename);

    /**jsdoc
    * Starts a specific trace event
    * @function Test.startTraceEvent
    * @param {string} name - Name of event
    */
    void startTraceEvent(QString name);

    /**jsdoc
    * Stop a specific name event
    * Using a filename with a .gz extension will automatically compress the output file
    * @function Test.endTraceEvent
    * @param {string} filename - Name of event
    */
    void endTraceEvent(QString name);

    /**jsdoc
     * Write detailed timing stats of next physics stepSimulation() to filename
     * @function Test.savePhysicsSimulationStats
     * @param {string} filename - Name of file to save to
     */
    void savePhysicsSimulationStats(QString filename);

    Q_INVOKABLE void profileRange(const QString& name, QScriptValue function);

    /**jsdoc
    * Clear all caches (menu command Reload Content)
    * @function Test.clearCaches
    */
    void clearCaches();

    /**jsdoc
    * Save a JSON object to a file in the test results location
    * @function Test.saveObject
    */
    void saveObject(QVariant v, const QString& filename);

    /**jsdoc
    * Maximizes the window
    * @function Test.showMaximized
    */
    void showMaximized();

private:
    bool waitForCondition(qint64 maxWaitMs, std::function<bool()> condition);
    QString _testResultsLocation;
};

#endif  // hifi_TestScriptingInterface_h

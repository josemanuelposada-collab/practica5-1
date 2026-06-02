#include "simulation.h"
#include "trajectorywidget.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Simulation simulation;
    simulation.setupDefaultScenario();
    simulation.run();

    const QString outputDir = QCoreApplication::applicationDirPath() + "/salida";
    const bool exportedText = simulation.exportTextFiles(outputDir);

    TrajectoryWidget window(simulation);
    const bool exportedImage = window.saveImage(outputDir + "/trayectorias.png");

    if (QCoreApplication::arguments().contains("--no-gui")) {
        return (exportedText && exportedImage) ? 0 : 1;
    }

    if (!exportedText || !exportedImage) {
        QMessageBox::warning(nullptr,
                             "Exportacion",
                             "La simulacion corrio, pero no se pudieron guardar todos los archivos de salida.");
    }

    window.show();
    return app.exec();
}

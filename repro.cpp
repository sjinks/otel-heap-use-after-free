#include <memory>
#include <stdexcept>
#include <utility>

#include <opentelemetry/exporters/ostream/log_record_exporter.h>
#include <opentelemetry/logs/logger_provider.h>
#include <opentelemetry/logs/provider.h>
#include <opentelemetry/sdk/logs/batch_log_record_processor_factory.h>
#include <opentelemetry/sdk/logs/batch_log_record_processor_options.h>
#include <opentelemetry/sdk/logs/exporter.h>
#include <opentelemetry/sdk/logs/logger_provider_factory.h>

int main()
{
    auto exporter = std::unique_ptr<opentelemetry::sdk::logs::LogRecordExporter>(
        new opentelemetry::exporter::logs::OStreamLogRecordExporter
    );
    opentelemetry::sdk::logs::BatchLogRecordProcessorOptions options{2048, std::chrono::milliseconds(5000), 512};
    auto processor = opentelemetry::sdk::logs::BatchLogRecordProcessorFactory::Create(std::move(exporter), options);

    std::shared_ptr<opentelemetry::sdk::logs::LoggerProvider> sdk_provider(
        opentelemetry::sdk::logs::LoggerProviderFactory::Create(std::move(processor))
    );

    const std::shared_ptr<opentelemetry::logs::LoggerProvider> &api_provider = sdk_provider;
    opentelemetry::logs::Provider::SetLoggerProvider(api_provider);

    try {
        throw std::runtime_error("error");
    }
    catch (const std::exception &e) {
        auto logger = opentelemetry::logs::Provider::GetLoggerProvider()->GetLogger("example");
        logger->Error(e.what());
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}

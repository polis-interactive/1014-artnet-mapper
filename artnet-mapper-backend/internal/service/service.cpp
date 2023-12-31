//
// Created by broosegoose on 7/23/23.
//

#include <cstdlib>

#include "service.hpp"

namespace service {

    ServicePtr Service::Create(const service::ServiceConfig &config) {
        auto embedded_service = std::make_shared<Service>();
        embedded_service->initialize(config);
        return embedded_service;
    }

    Service::Service(): _is_started(false) {}

    void Service::initialize(const service::ServiceConfig &config) {
        _asio_context = infrastructure::AsioContext::Create(config.asio_context_config);
        _art_net = infrastructure::ArtNet::Create(config.art_net_config, _asio_context->GetContext());
        _graphics = infrastructure::Graphics::Create(config.graphics_config, shared_from_this());
        _controls = infrastructure::Controls::Create(config.controls_config, shared_from_this());
        _run_pipeline = config.service_run_pipeline;
        _allow_reset = config.service_allow_reset;
    }

    void Service::Start() {
        if (_is_started) {
            return;
        }
        _asio_context->Start();
        _art_net->Start();
        _graphics->Start();
        _controls->Start();
        _is_started = true;
    }

    void Service::Stop() {
        if (!_is_started) {
            return;
        }
        _controls->Stop();
        _graphics->Stop();
        // let it ship out last frame
        std::this_thread::sleep_for(100ms);
        _art_net->Stop();
        _asio_context->Stop();
        _is_started = false;
    }

    void Service::Unset() {
        _art_net.reset();
        _asio_context.reset();
        _controls.reset();
        _graphics.reset();
    }

    void Service::PostGraphicsUpdate(utility::SizedBufferPtr &&pixels) {
        if (_run_pipeline) {
            _art_net->Post(std::move(pixels));
        }
    }

    void Service::RequestReboot() {
        // should turn off iono led
        _controls->Stop();
        // wait for last frame to ship out
        std::this_thread::sleep_for(500ms);
        // do the reboot
        if (_allow_reset) {
            std::system("/sbin/shutdown -r now");
        }
    }

    void Service::PostPotentiometerUpdate(const float new_pot_read) {
        if (_run_pipeline) {
            _graphics->PostBrightness(new_pot_read);
        }
    }

}
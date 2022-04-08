/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "pybind11/pybind11.h"
#include "pybind11_abseil/status_casters.h"  // from @pybind11_abseil
#include "pybind11_protobuf/native_proto_caster.h"  // from @pybind11_protobuf
#include "tensorflow_lite_support/cc/port/statusor.h"
#include "tensorflow_lite_support/cc/task/audio/audio_embedder.h"
#include "tensorflow_lite_support/cc/task/audio/core/audio_buffer.h"
#include "tensorflow_lite_support/python/task/core/pybinds/task_utils.h"

namespace tflite {
namespace task {
namespace audio {

namespace {
namespace py = ::pybind11;
using PythonBaseOptions = ::tflite::python::task::core::BaseOptions;
using CppBaseOptions = ::tflite::task::core::BaseOptions;
}  // namespace

PYBIND11_MODULE(_pywrap_audio_embedder, m) {
  // python wrapper for C++ AudioEmbedder class which shouldn't be directly used
  // by the users.
  pybind11::google::ImportStatusModule();
  pybind11_protobuf::ImportNativeProtoCasters();

  py::class_<AudioEmbedder>(m, "AudioEmbedder")
      .def_static(
          "create_from_options",
          [](const PythonBaseOptions& base_options,
             const processor::EmbeddingOptions& embedding_options) {
            AudioEmbedderOptions options;
            auto cpp_base_options =
                core::convert_to_cpp_base_options(base_options);

            options.set_allocated_base_options(cpp_base_options.release());
            options.add_embedding_options()->CopyFrom(embedding_options);
            return AudioEmbedder::CreateFromOptions(options);
          })
      .def_static("cosine_similarity", &AudioEmbedder::CosineSimilarity)
      .def("embed", &AudioEmbedder::Embed)
      .def("get_embedding_dimension", &AudioEmbedder::GetEmbeddingDimension)
      .def("get_number_of_output_layers",
           &AudioEmbedder::GetNumberOfOutputLayers)
      .def("get_required_audio_format", &AudioEmbedder::GetRequiredAudioFormat)
      .def("get_required_input_buffer_size",
           &AudioEmbedder::GetRequiredInputBufferSize);
}

}  // namespace audio
}  // namespace task
}  // namespace tflite

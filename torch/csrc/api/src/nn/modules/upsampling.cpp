#include <torch/nn/modules/upsampling.h>

#include <string>

namespace F = torch::nn::functional;

namespace torch {
namespace nn {

UpsampleImpl::UpsampleImpl(const UpsampleOptions& options_) // NOLINT(modernize-pass-by-value)
    : options(options_) {}

void UpsampleImpl::reset() {}

void UpsampleImpl::pretty_print(std::ostream& stream) const {
  stream << "torch::nn::Upsample(";
  if (!options.scale_factor().empty()) {
    stream << "scale_factor=" << at::ArrayRef<double>(options.scale_factor());
  } else {
    stream << "size=" << at::ArrayRef<int64_t>(options.size());
  }
  stream << ", mode=" << c10::visit(enumtype::enum_name{}, options.mode()) << ")";
}

Tensor UpsampleImpl::forward(const Tensor& input) {
  InterpolateOptions::mode_t mode;
  if (c10::get_if<enumtype::kNearest>(&options.mode())) {
    mode = torch::kNearest;
  } else if (c10::get_if<enumtype::kLinear>(&options.mode())) {
    mode = torch::kLinear;
  } else if (c10::get_if<enumtype::kBilinear>(&options.mode())) {
    mode = torch::kBilinear;
  } else if (c10::get_if<enumtype::kBicubic>(&options.mode())) {
    mode = torch::kBicubic;
  } else if (c10::get_if<enumtype::kTrilinear>(&options.mode())) {
    mode = torch::kTrilinear;
  }

  return F::interpolate(
      input,
      InterpolateOptions()
          .size(options.size())
          .scale_factor(options.scale_factor())
          .mode(mode)
          .align_corners(options.align_corners()));
}

} // namespace nn
} // namespace torch

#!/usr/bin/env lua

util = {}

ffi = require('ffi')

util.tensor_type = {
    ['unsigned char'] = 'torch.ByteTensor',
    ['char'] = 'torch.CharTensor',
    ['short'] = 'torch.ShortTensor',
    ['int'] = 'torch.IntTensor',
    ['long'] = 'torch.LongTensor',
    ['float'] ='torch.FloatTensor',
    ['double'] = 'torch.DoubleTensor'
}

function util.tensor2cdata(data, data_type)
    data_type = data_type or 'float'
    tensor_type = util.tensor_type[data_type]
    return torch.Tensor(data):contiguous():type(tensor_type):data()
end

function util.cdata2tensor(cdata, sizes, data_type)
    data_type = data_type or 'float'
    tensor_type = util.tensor_type[data_type]
    data = torch.Tensor(sizes):type(tensor_type)
    ffi.copy(data:data(), cdata, data:nElement() * ffi.sizeof(data_type))
    return data
end

function util.Set(list)
  local set = {}
  for _, l in ipairs(list) do set[l] = true end
  return set
end

return util

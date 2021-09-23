# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: cardano.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from . import common_pb2 as common__pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='cardano.proto',
  package='shiftcrypto.bitbox02',
  syntax='proto3',
  serialized_pb=_b('\n\rcardano.proto\x12\x14shiftcrypto.bitbox02\x1a\x0c\x63ommon.proto\"\x9e\x01\n\x13\x43\x61rdanoScriptConfig\x12\x43\n\x07pkh_skh\x18\x01 \x01(\x0b\x32\x30.shiftcrypto.bitbox02.CardanoScriptConfig.PkhSkhH\x00\x1a\x38\n\x06PkhSkh\x12\x17\n\x0fkeypath_payment\x18\x01 \x03(\r\x12\x15\n\rkeypath_stake\x18\x02 \x03(\rB\x08\n\x06\x63onfig\"\xa1\x01\n\x15\x43\x61rdanoAddressRequest\x12\x35\n\x07network\x18\x01 \x01(\x0e\x32$.shiftcrypto.bitbox02.CardanoNetwork\x12\x0f\n\x07\x64isplay\x18\x02 \x01(\x08\x12@\n\rscript_config\x18\x03 \x01(\x0b\x32).shiftcrypto.bitbox02.CardanoScriptConfig\"[\n\x0e\x43\x61rdanoRequest\x12>\n\x07\x61\x64\x64ress\x18\x01 \x01(\x0b\x32+.shiftcrypto.bitbox02.CardanoAddressRequestH\x00\x42\t\n\x07request\"O\n\x0f\x43\x61rdanoResponse\x12\x30\n\x03pub\x18\x01 \x01(\x0b\x32!.shiftcrypto.bitbox02.PubResponseH\x00\x42\n\n\x08response*8\n\x0e\x43\x61rdanoNetwork\x12\x12\n\x0e\x43\x61rdanoMainnet\x10\x00\x12\x12\n\x0e\x43\x61rdanoTestnet\x10\x01\x62\x06proto3')
  ,
  dependencies=[common__pb2.DESCRIPTOR,])
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

_CARDANONETWORK = _descriptor.EnumDescriptor(
  name='CardanoNetwork',
  full_name='shiftcrypto.bitbox02.CardanoNetwork',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='CardanoMainnet', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='CardanoTestnet', index=1, number=1,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=552,
  serialized_end=608,
)
_sym_db.RegisterEnumDescriptor(_CARDANONETWORK)

CardanoNetwork = enum_type_wrapper.EnumTypeWrapper(_CARDANONETWORK)
CardanoMainnet = 0
CardanoTestnet = 1



_CARDANOSCRIPTCONFIG_PKHSKH = _descriptor.Descriptor(
  name='PkhSkh',
  full_name='shiftcrypto.bitbox02.CardanoScriptConfig.PkhSkh',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='keypath_payment', full_name='shiftcrypto.bitbox02.CardanoScriptConfig.PkhSkh.keypath_payment', index=0,
      number=1, type=13, cpp_type=3, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='keypath_stake', full_name='shiftcrypto.bitbox02.CardanoScriptConfig.PkhSkh.keypath_stake', index=1,
      number=2, type=13, cpp_type=3, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=146,
  serialized_end=202,
)

_CARDANOSCRIPTCONFIG = _descriptor.Descriptor(
  name='CardanoScriptConfig',
  full_name='shiftcrypto.bitbox02.CardanoScriptConfig',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='pkh_skh', full_name='shiftcrypto.bitbox02.CardanoScriptConfig.pkh_skh', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[_CARDANOSCRIPTCONFIG_PKHSKH, ],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
    _descriptor.OneofDescriptor(
      name='config', full_name='shiftcrypto.bitbox02.CardanoScriptConfig.config',
      index=0, containing_type=None, fields=[]),
  ],
  serialized_start=54,
  serialized_end=212,
)


_CARDANOADDRESSREQUEST = _descriptor.Descriptor(
  name='CardanoAddressRequest',
  full_name='shiftcrypto.bitbox02.CardanoAddressRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='network', full_name='shiftcrypto.bitbox02.CardanoAddressRequest.network', index=0,
      number=1, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='display', full_name='shiftcrypto.bitbox02.CardanoAddressRequest.display', index=1,
      number=2, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='script_config', full_name='shiftcrypto.bitbox02.CardanoAddressRequest.script_config', index=2,
      number=3, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=215,
  serialized_end=376,
)


_CARDANOREQUEST = _descriptor.Descriptor(
  name='CardanoRequest',
  full_name='shiftcrypto.bitbox02.CardanoRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='address', full_name='shiftcrypto.bitbox02.CardanoRequest.address', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
    _descriptor.OneofDescriptor(
      name='request', full_name='shiftcrypto.bitbox02.CardanoRequest.request',
      index=0, containing_type=None, fields=[]),
  ],
  serialized_start=378,
  serialized_end=469,
)


_CARDANORESPONSE = _descriptor.Descriptor(
  name='CardanoResponse',
  full_name='shiftcrypto.bitbox02.CardanoResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='pub', full_name='shiftcrypto.bitbox02.CardanoResponse.pub', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
    _descriptor.OneofDescriptor(
      name='response', full_name='shiftcrypto.bitbox02.CardanoResponse.response',
      index=0, containing_type=None, fields=[]),
  ],
  serialized_start=471,
  serialized_end=550,
)

_CARDANOSCRIPTCONFIG_PKHSKH.containing_type = _CARDANOSCRIPTCONFIG
_CARDANOSCRIPTCONFIG.fields_by_name['pkh_skh'].message_type = _CARDANOSCRIPTCONFIG_PKHSKH
_CARDANOSCRIPTCONFIG.oneofs_by_name['config'].fields.append(
  _CARDANOSCRIPTCONFIG.fields_by_name['pkh_skh'])
_CARDANOSCRIPTCONFIG.fields_by_name['pkh_skh'].containing_oneof = _CARDANOSCRIPTCONFIG.oneofs_by_name['config']
_CARDANOADDRESSREQUEST.fields_by_name['network'].enum_type = _CARDANONETWORK
_CARDANOADDRESSREQUEST.fields_by_name['script_config'].message_type = _CARDANOSCRIPTCONFIG
_CARDANOREQUEST.fields_by_name['address'].message_type = _CARDANOADDRESSREQUEST
_CARDANOREQUEST.oneofs_by_name['request'].fields.append(
  _CARDANOREQUEST.fields_by_name['address'])
_CARDANOREQUEST.fields_by_name['address'].containing_oneof = _CARDANOREQUEST.oneofs_by_name['request']
_CARDANORESPONSE.fields_by_name['pub'].message_type = common__pb2._PUBRESPONSE
_CARDANORESPONSE.oneofs_by_name['response'].fields.append(
  _CARDANORESPONSE.fields_by_name['pub'])
_CARDANORESPONSE.fields_by_name['pub'].containing_oneof = _CARDANORESPONSE.oneofs_by_name['response']
DESCRIPTOR.message_types_by_name['CardanoScriptConfig'] = _CARDANOSCRIPTCONFIG
DESCRIPTOR.message_types_by_name['CardanoAddressRequest'] = _CARDANOADDRESSREQUEST
DESCRIPTOR.message_types_by_name['CardanoRequest'] = _CARDANOREQUEST
DESCRIPTOR.message_types_by_name['CardanoResponse'] = _CARDANORESPONSE
DESCRIPTOR.enum_types_by_name['CardanoNetwork'] = _CARDANONETWORK

CardanoScriptConfig = _reflection.GeneratedProtocolMessageType('CardanoScriptConfig', (_message.Message,), dict(

  PkhSkh = _reflection.GeneratedProtocolMessageType('PkhSkh', (_message.Message,), dict(
    DESCRIPTOR = _CARDANOSCRIPTCONFIG_PKHSKH,
    __module__ = 'cardano_pb2'
    # @@protoc_insertion_point(class_scope:shiftcrypto.bitbox02.CardanoScriptConfig.PkhSkh)
    ))
  ,
  DESCRIPTOR = _CARDANOSCRIPTCONFIG,
  __module__ = 'cardano_pb2'
  # @@protoc_insertion_point(class_scope:shiftcrypto.bitbox02.CardanoScriptConfig)
  ))
_sym_db.RegisterMessage(CardanoScriptConfig)
_sym_db.RegisterMessage(CardanoScriptConfig.PkhSkh)

CardanoAddressRequest = _reflection.GeneratedProtocolMessageType('CardanoAddressRequest', (_message.Message,), dict(
  DESCRIPTOR = _CARDANOADDRESSREQUEST,
  __module__ = 'cardano_pb2'
  # @@protoc_insertion_point(class_scope:shiftcrypto.bitbox02.CardanoAddressRequest)
  ))
_sym_db.RegisterMessage(CardanoAddressRequest)

CardanoRequest = _reflection.GeneratedProtocolMessageType('CardanoRequest', (_message.Message,), dict(
  DESCRIPTOR = _CARDANOREQUEST,
  __module__ = 'cardano_pb2'
  # @@protoc_insertion_point(class_scope:shiftcrypto.bitbox02.CardanoRequest)
  ))
_sym_db.RegisterMessage(CardanoRequest)

CardanoResponse = _reflection.GeneratedProtocolMessageType('CardanoResponse', (_message.Message,), dict(
  DESCRIPTOR = _CARDANORESPONSE,
  __module__ = 'cardano_pb2'
  # @@protoc_insertion_point(class_scope:shiftcrypto.bitbox02.CardanoResponse)
  ))
_sym_db.RegisterMessage(CardanoResponse)


# @@protoc_insertion_point(module_scope)

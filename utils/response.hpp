#pragma once

enum class ResponseCode {
  Success, // successfully done
  Failure, // restricted / failed action
  Error,   // fatal error (invalid parameter, etc)
};

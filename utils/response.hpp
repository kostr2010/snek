#pragma once

enum class ResponseCode {
  Success, // successfully done
  Failure, // restricted action
  Error,   // fatal error (abort program)
};

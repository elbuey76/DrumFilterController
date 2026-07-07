#include <string.h>
#include <unity.h>

#include "sim/SimulatorCommands.h"
#include "sim/SimulatorLineBuffer.h"

namespace {

struct StreamHarness {
  char storage[16]{};
  SimulatorLineBuffer buffer{storage, sizeof(storage)};
  InputsSnapshot inputs;
  unsigned int linesHandled = 0;
  unsigned int overflows = 0;
  SimulatorCommandAction lastAction = SimulatorCommandAction::NONE;

  void feed(const char* text) {
    for (const char* cursor = text; *cursor != '\0'; ++cursor) {
      const SimulatorLineEvent event = buffer.push(*cursor);
      if (event == SimulatorLineEvent::LINE_READY) {
        lastAction = applySimulatorCommand(buffer.line(), inputs).action;
        ++linesHandled;
        buffer.reset();
      } else if (event == SimulatorLineEvent::BUFFER_OVERFLOW) {
        ++overflows;
      }
    }
  }
};

}  // namespace

void setUp() {}

void tearDown() {}

void test_line_ready_on_lf_applies_command() {
  StreamHarness h;

  h.feed("lavage on\n");

  TEST_ASSERT_EQUAL_UINT(1, h.linesHandled);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_EP_LAVAGE, h.lastAction);
  TEST_ASSERT_TRUE(h.inputs.epLavage);
  TEST_ASSERT_EQUAL_UINT(0, h.overflows);
  TEST_ASSERT_EQUAL_UINT(0, h.buffer.length());
}

void test_crlf_ignores_carriage_return() {
  StreamHarness h;

  h.feed("capot open\r\n");

  TEST_ASSERT_EQUAL_UINT(1, h.linesHandled);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_CAPOT_OUVERT, h.lastAction);
  TEST_ASSERT_TRUE(h.inputs.capotOuvert);
  TEST_ASSERT_EQUAL_UINT(0, h.overflows);
}

void test_multiple_commands_in_same_stream_are_processed_in_order() {
  StreamHarness h;

  h.feed("manual\nrincage on\n");

  TEST_ASSERT_EQUAL_UINT(2, h.linesHandled);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_MANU_RINCAGE, h.lastAction);
  TEST_ASSERT_FALSE(h.inputs.modeAuto);
  TEST_ASSERT_FALSE(h.inputs.modeMaintenance);
  TEST_ASSERT_TRUE(h.inputs.btnManuRincage);
}

void test_partial_line_is_buffered_until_newline() {
  StreamHarness h;

  h.feed("test");

  TEST_ASSERT_EQUAL_UINT(0, h.linesHandled);
  TEST_ASSERT_EQUAL_UINT(4, h.buffer.length());
  TEST_ASSERT_EQUAL_STRING("test", h.buffer.line());
  TEST_ASSERT_FALSE(h.inputs.btnTestLavage);

  h.feed("\n");

  TEST_ASSERT_EQUAL_UINT(1, h.linesHandled);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::PULSE_TEST_LAVAGE, h.lastAction);
  TEST_ASSERT_TRUE(h.inputs.btnTestLavage);
}

void test_overflow_resets_buffer_and_next_command_still_works() {
  StreamHarness h;

  h.feed("12345678901234567890");

  TEST_ASSERT_EQUAL_UINT(0, h.linesHandled);
  TEST_ASSERT_EQUAL_UINT(1, h.overflows);
  TEST_ASSERT_EQUAL_UINT(4, h.buffer.length());
  TEST_ASSERT_EQUAL_STRING("7890", h.buffer.line());

  h.buffer.reset();
  h.feed("auto\n");

  TEST_ASSERT_EQUAL_UINT(1, h.linesHandled);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::MODE_AUTO, h.lastAction);
  TEST_ASSERT_TRUE(h.inputs.modeAuto);
  TEST_ASSERT_FALSE(h.inputs.modeMaintenance);
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  UNITY_BEGIN();
  RUN_TEST(test_line_ready_on_lf_applies_command);
  RUN_TEST(test_crlf_ignores_carriage_return);
  RUN_TEST(test_multiple_commands_in_same_stream_are_processed_in_order);
  RUN_TEST(test_partial_line_is_buffered_until_newline);
  RUN_TEST(test_overflow_resets_buffer_and_next_command_still_works);
  return UNITY_END();
}

const { Bar } = require('.');

async function createBar() {
  const bar = new Bar();
  console.log(`  Bar: ${bar.id}`);
  return bar;
}

async function createFoo() {
  const bar = await createBar();
  return new Promise(resolve => {
    bar.onfoo = foo => {
      console.log(`    Foo: ${foo.id}`);
      resolve(foo);
      bar.stop();
    };
    bar.createFoo();
  });
}

async function test(n) {
  const foos = [];
  for (let i = 0; i < n; i++) {
    const foo = await createFoo();
    foos.push(foo);
    foo.stop();
  }
}

(async () => {
  for (let i = 0; i < 100; i++) {
    console.log(`Test: ${i}`);
    await test(1000);
  }
})().catch(error => {
  console.error(error.stack);
  process.exit(1);
});
